#include <iostream>
#include <errno.h>
#include <libpi/channel_mq.hpp>
#include <libpi/common.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace libpi;
using namespace std;

unsigned int Channel_MQ::ourQueueCounter=0;

Channel_MQ::Channel_MQ(const string &queue) // {{{
: myUnlink(false)
{
  if (queue=="")
  { // Create new uniqie name
    myName="/libpi_"+int2str(getpid())+"_"+int2str(++ourQueueCounter);
  }
  else if (queue[0]=='/')
  { // Use queue as name
    myName=queue;
  }
  else
  { // Create name based on queue
    myName="/libpi_static_"+queue;
  }
  myQueue=mq_open(myName.c_str(),O_RDWR | O_CREAT, 0600, NULL);
  if (myQueue==-1) // Error
    throw (string)"Unable to create message queue: " + myName + "\n"
                + "Error was: " + strerror(errno);
  mq_getattr(myQueue,&myAttributes);
} // }}}

Channel_MQ::~Channel_MQ() // {{{
{
  mq_close(myQueue);
  if (myUnlink)
    mq_unlink(myName.c_str());
} // }}}

void Channel_MQ::Unlink() // {{{
{
  myUnlink=true;
} // }}}

void Channel_MQ::Send(Message &msg) // {{{
{
  int pos=0;
  int size=msg.GetSize();
  const char *data=msg.GetData();
  mq_send(myQueue, (const char*)&size, sizeof(size),0); // Send Size
  while (pos<size)
  { int delta=size-pos<myAttributes.mq_msgsize?size-pos:myAttributes.mq_msgsize;
    int res=mq_send(myQueue,data+pos,delta,0);
    if (res!=0)
      throw (string)"Unable to send on message queue: " + myName + "\n"
                   + "Error was: " + strerror(errno);
    pos+=delta;
  }
} // }}}

void Channel_MQ::Receive(Message &msg) // {{{
{
  int pos=0;
  int size;
  char *data = new char[myAttributes.mq_msgsize];
  int delta=mq_receive(myQueue, data, myAttributes.mq_msgsize,0); // Receive Size
  if (delta != sizeof(size))
    throw (string) "Channel_MQ::Receive: Wrong header size";
  memcpy((char*)&size,data,sizeof(size));
  while (pos<size)
  { int delta=mq_receive(myQueue,data,myAttributes.mq_msgsize,0);
    if (delta==-1)
      throw (string) "Channel_MQ::Receive: Unable to receive on message queue: " + myName + "\n"
                   + "Error was: " + strerror(errno);
    if (delta>size-pos)
      throw (string) "Channel_MQ::Receive: Eeceived too much data!" + "\n"
                   + "Remaining size: " + int2str(size-pos) + "\n"
                   + "Message size: " + int2str(delta);
    msg.AddData(data,delta);
    pos+=delta;
  }
  delete [] data;
} // }}}

void Channel_MQ::SingleSend(Message &msg) // {{{
{
  int size=msg.GetSize();
  if (sizeof(size)+size>myAttributes.mq_msgsize)
    throw (string)"Channel_MQ::SingleSend: Meggage is too long for a single message";
  char *data = new char[myAttributes.mq_msgsize];
  memcpy(data,(const char*)&size,sizeof(size));
  memcpy(data+sizeof(size),msg.GetData(),size);
  mq_send(myQueue, data, size+4,0); // Send Size
  delete [] data;
} // }}}

void Channel_MQ::SingleReceive(Message &msg) // {{{
{
  char *data = new char[myAttributes.mq_msgsize];
  mq_receive(myQueue, data, myAttributes.mq_msgsize,0);
  int size=*((int*)data);
  msg.AddData(data+sizeof(size),size);
  delete [] data;
} // }}}

string Channel_MQ::GetAddress() // {{{
{ return myName;
} // }}}
