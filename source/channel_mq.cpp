#include <libpi/channel_mq.hpp>
#include <libpi/common.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
using namespace libpi;
using namespace std;

Channel_MQ::Channel_MQ(const string &queue) // {{{
{
  if (queue=="")
  { // Create new uniqie name
    myName="/libpi_"+int2str(getpid())+"_"+int2str(++ourQueueCounter);
  }
  else
  { // Use given name for queue
    myName="/libpi_static_"+queue;
  }
  myQueue=mq_open(myName.c_str(),O_RDWR | O_CREAT);
  mq_getattr(myQueue,&myAttributes);
} // }}}

Channel_MQ::~Channel_MQ() // {{{
{
  mq_close(myQueue);
} // }}}

void Channel_MQ::Send(Message &msg) // {{{
{
  int pos=0;
  int size=msg.GetSize();
  const char *data=msg.GetData();
  mq_send(myQueue, (const char*)&size, 4,0); // Send Size
  while (pos<size)
  { int delta=size-pos<myAttributes.mq_msgsize?size-pos:myAttributes.mq_msgsize;
    mq_send(myQueue,data+pos,delta,0);
    pos+=delta;
  }
} // }}}

void Channel_MQ::Receive(Message &msg) // {{{
{
  int pos=0;
  int size;
  char *data = new char[myAttributes.mq_msgsize];
  mq_receive(myQueue, (char*)&size, 4,0); // Receive Size
  while (pos<size)
  { int delta=size-pos<myAttributes.mq_msgsize?size-pos:myAttributes.mq_msgsize;
    mq_receive(myQueue,data,delta,0);
    pos+=delta;
    msg.AddData(data,delta);
  }
} // }}}

void Channel_MQ::SingleReceive(Message &msg) // {{{
{
  char *data = new char[myAttributes.mq_msgsize];
  mq_receive(myQueue, data, myAttributes.mq_msgsize,0);
  int size=*((int*)data);
  msg.AddData(data+4,size);
  delete [] data;
} // }}}

void Channel_MQ::SingleSend(Message &msg) // {{{
{
  int size=msg.GetSize();
  if (size+4>myAttributes.mq_msgsize)
    throw "Channel::SingleSend message exceeds maximum size";
  char *data = new char[myAttributes.mq_msgsize];
  const char *msgdata=msg.GetData();
  memcpy(data,(const char*)&size,4);
  memcpy(data+4,msgdata,size);
  mq_send(myQueue, data, size+4,0); // Send Size
  delete [] data;
} // }}}

string Channel_MQ::GetAddress() // {{{
{ return myName;
} // }}}
