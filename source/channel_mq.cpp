#include <iostream>
#include <errno.h>
#include <libpi/channel_mq.hpp>
#include <libpi/common.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
using namespace libpi;
using namespace std;

unsigned int Channel_MQ::ourQueueCounter=0;
const int Channel_MQ::ourMessageLength=1024*8;

Channel_MQ::Channel_MQ(int key) // {{{
: myUnlink(false)
{
  if (key==-1)
  { // Create new uniqie name
    myKey=getpid()*1000+(++ourQueueCounter);
  }
  else
  { // Create name based on queue
    myKey=key;
  }
  cout << "msgget(" << myKey << ",00600 | IPC_CREAT)" << endl;
  myQueue=msgget(myKey,00600 | IPC_CREAT);
  cout << "msgget(" << myKey << ",00600 | IPC_CREAT)=" << myQueue << endl;
  if (myQueue==-1) // Error
    throw (string)"Unable to create message queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  if (msgctl(myQueue,IPC_STAT, &myAttributes)==-1)
    throw (string)"Unable to stat queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  cout << "Original msg_qbytes: " << myAttributes.msg_qbytes << endl;
  cout << "Setting msg_qbytes to 100MB";
  myAttributes.msg_qbytes=1024*1024*100;
  if (msgctl(myQueue,IPC_SET, &myAttributes)==-1)
    throw (string)"Unable to set msg_qbytes on queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
} // }}}

Channel_MQ::~Channel_MQ() // {{{
{
  if (myUnlink)
    msgctl(myQueue, IPC_RMID, NULL);
} // }}}

void Channel_MQ::Unlink() // {{{
{
  myUnlink=true;
} // }}}

void Channel_MQ::Send(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " << " << msg.GetData() << endl;
  int pos=0;
  int size=msg.GetSize();
  const char *data=msg.GetData();
  if (msgsnd(myQueue, (const char*)&size, sizeof(size),0)!=0) // Send Size
      throw (string)"Channel_MQ::Send: Unable to send on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
  while (pos<size)
  { int delta=size-pos<ourMessageLength?size-pos:ourMessageLength;
    if (msgsnd(myQueue,data+pos,delta,0)!=0)
      throw (string)"Channel_MQ::Send: Unable to send on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
    pos+=delta;
  }
} // }}}

void Channel_MQ::Receive(Message &msg) // {{{
{
  int pos=0;
  int size;
  char *data = new char[ourMessageLength];
  int delta=msgrcv(myQueue,data,ourMessageLength,0,0); // Receive Size
  if (delta != sizeof(size))
    throw (string) "Channel_MQ::Receive: Wrong header size";
  memcpy((char*)&size,data,sizeof(size));
  while (pos<size)
  { int delta=msgrcv(myQueue,data,ourMessageLength,0,0);
    if (delta==-1)
      throw (string) "Channel_MQ::Receive: Unable to receive on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
    if (delta>size-pos)
      throw (string) "Channel_MQ::Receive: Received too much data!" + "\n"
                   + "Remaining size: " + int2str(size-pos) + "\n"
                   + "Message size: " + int2str(delta);
    msg.AddData(data,delta);
    pos+=delta;
  }
  delete [] data;
  cout << GetAddress() << "(" << myQueue << ") " << " >> " << msg.GetData() << endl;
} // }}}

void Channel_MQ::SingleSend(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " « " << msg.GetData() << endl;
  int size=msg.GetSize();
  if (sizeof(size)+size>ourMessageLength)
    throw (string)"Channel_MQ::SingleSend: Meggage is too long for a single message";
  char *data = new char[ourMessageLength];
  memcpy(data,(const char*)&size,sizeof(size));
  memcpy(data+sizeof(size),msg.GetData(),size);
  msgsnd(myQueue, data, size+4,0); // Send Size and Message
  delete [] data;
} // }}}

void Channel_MQ::SingleReceive(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " » ..." << endl;
  char *data = new char[ourMessageLength];
  msgrcv(myQueue,data,ourMessageLength,0,0);
  int size=*((int*)data);
  msg.AddData(data+sizeof(size),size);
  delete [] data;
  cout << GetAddress() << "(" << myQueue << ") " << " » " << msg.GetData() << endl;
} // }}}

string Channel_MQ::GetAddress() // {{{
{ return (string)"mqchannel://"+int2str(myKey);
} // }}}
