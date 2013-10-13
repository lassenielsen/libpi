#include <iostream>
#include <errno.h>
#include <libpi/channel_mq.hpp>
#include "common.cpp"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
using namespace libpi;
using namespace std;

unsigned int Channel_MQ::ourQueueCounter=0;
const int Channel_MQ::ourMessageLength=1024*8;

Channel_MQ::Channel_MQ(const string &address) // {{{
: myUnlink(false)
{
  // Extract key
  string prefix("mqchannel://");
  if (address.compare(0, prefix.size(), prefix))
      throw (string)"Channel_MQ::Channel_MQ: Address is not an mqchannel: " + address;
  int key=str2int(address.substr(prefix.size()));
  if (key==-1)
  { // Create new uniqie name
    myKey=getpid()*1000+(++ourQueueCounter);
  }
  else
  { // Create name based on queue
    myKey=key;
  }
  //cout << "msgget(" << myKey << ",00600 | IPC_CREAT)" << endl;
  myQueue=msgget(myKey,00600 | IPC_CREAT);
  //cout << "msgget(" << myKey << ",00600 | IPC_CREAT)=" << myQueue << endl;
  if (myQueue==-1) // Error
    throw (string)"Unable to create message queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  if (msgctl(myQueue,IPC_STAT, &myAttributes)==-1)
    throw (string)"Unable to stat queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  //cout << "Original msg_qbytes: " << myAttributes.msg_qbytes << endl;
  //cout << "Setting msg_qbytes to 100MB" << endl;
  myAttributes.msg_qbytes=1024*1024*100;
  if (msgctl(myQueue,IPC_SET, &myAttributes)==-1)
    throw (string)"Unable to set msg_qbytes on queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
} // }}}

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
  //cout << "msgget(" << myKey << ",00600 | IPC_CREAT)" << endl;
  myQueue=msgget(myKey,00600 | IPC_CREAT);
  //cout << "msgget(" << myKey << ",00600 | IPC_CREAT)=" << myQueue << endl;
  if (myQueue==-1) // Error
    throw (string)"Unable to create message queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  if (msgctl(myQueue,IPC_STAT, &myAttributes)==-1)
    throw (string)"Unable to stat queue: " + int2str(myKey) + "\n"
                + "Error was: " + strerror(errno);
  //cout << "Original msg_qbytes: " << myAttributes.msg_qbytes << endl;
  //cout << "Setting msg_qbytes to 100MB" << endl;
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
  long size=msg.GetSize();
  struct {long mtype; char mtext[ourMessageLength];} message;
  message.mtype=1;
  memcpy(&message.mtext[0],(const char*)&size,sizeof(long));
  if (msgsnd(myQueue, &message, sizeof(size),0)!=0) // Send Size
      throw (string)"Channel_MQ::Send: Unable to send on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
  while (pos<size)
  { int delta=size-pos<ourMessageLength?size-pos:ourMessageLength;
    memcpy(&message.mtext[0],msg.GetData()+pos,delta);
    if (msgsnd(myQueue,(void*)&message,delta,0)!=0)
      throw (string)"Channel_MQ::Send: Unable to send on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
    pos+=delta;
  }
} // }}}

void Channel_MQ::Receive(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " >> " << "..." << endl;
  //cout << "rcv1" << endl;
  int pos=0;
  long size;
  struct {long mtype; char mtext[ourMessageLength];} message;
  int delta=msgrcv(myQueue,(void*)&message,ourMessageLength,0,0); // Receive Size
  cout << "Delta: " << delta;
  cout << "Size: " << *((long*)&message.mtext[0]) << endl;
  if (delta != sizeof(size))
    throw (string) "Channel_MQ::Receive: Wrong header size on channel key " + int2str(myKey) + "\n Expected: " + int2str(sizeof(size)) + "\n Found: " + int2str(delta);
  memcpy((char*)&size,&message.mtext[0],sizeof(size));
  //cout << "rcv2 size: " << size << endl;
  while (pos<size)
  { 
    cout << "rcv3 pos: " << pos << ", size: " << size << endl;
    int delta=msgrcv(myQueue,(void*)&message,ourMessageLength,0,0);
    if (delta==-1)
      throw (string) "Channel_MQ::Receive: Unable to receive on message queue: " + int2str(myKey) + "\n"
                   + "Error was: " + strerror(errno);
    cout << "rcv4 size: " << size << ", pos: " << pos << ", delta: " << delta << endl;
    if (delta>size-pos)
      throw (string) "Channel_MQ::Receive: Received too much data!" + "\n"
                   + "Remaining size: " + int2str(size-pos) + "\n"
                   + "Message size: " + int2str(delta);
    cout << "rcv5 size: " << size << ", pos: " << pos << ", delta: " << delta << endl;
    msg.AddData(&message.mtext[0],delta);
    pos+=delta;
    cout << "rcv6 size: " << size << ", pos: " << pos << ", delta: " << delta << "Buffer: " << msg.GetData() << endl;
  }
  //cout << GetAddress() << "(" << myQueue << ") " << " >> " << msg.GetData() << endl;
} // }}}

void Channel_MQ::SingleSend(Message &msg) // {{{
{
  //cout << GetAddress() << "(" << myQueue << ") " << " « " << msg.GetData() << endl;
  if (msg.GetSize()>ourMessageLength)
    throw (string)"Channel_MQ::SingleSend: Meggage is too long for a single message";
  struct {long mtype; char mtext[ourMessageLength];} message;
  message.mtype=1;
  memcpy(&message.mtext[0],msg.GetData(),msg.GetSize());
  msgsnd(myQueue, &message, msg.GetSize(),0); // Send Size and Message
} // }}}

void Channel_MQ::SingleReceive(Message &msg) // {{{
{
  //cout << GetAddress() << "(" << myQueue << ") " << " » ..." << endl;
  struct {long mtype; char mtext[ourMessageLength];} message;
  int size=msgrcv(myQueue,&message,ourMessageLength,0,0);
  if (size==-1) // error
    throw (string)"Unable to receive message on queue: " + int2str(myKey) + "(" + int2str(myQueue) + ")\n"
                + "Error was: " + strerror(errno);
    msg.AddData(&message.mtext[0],size);
  //cout << GetAddress() << "(" << myQueue << ") " << " » " << msg.GetData() << endl;
} // }}}

string Channel_MQ::GetAddress() const // {{{
{ return (string)"mqchannel://"+int2str(myKey);
} // }}}
