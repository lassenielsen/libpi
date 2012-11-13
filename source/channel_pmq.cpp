#include <iostream>
#include <errno.h>
#include <libpi/channel_pmq.hpp>
#include <libpi/common.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
using namespace libpi;
using namespace std;

int init_rlimit() // Set message queue limits as high as possible {{{
{ rlimit limit;
  int res1 = getrlimit(RLIMIT_MSGQUEUE,&limit);
  cout << "Orig rlim_cur: " << limit.rlim_cur << endl;
  cout << "Orig rlim_max: " << limit.rlim_max << endl;
  limit.rlim_max=RLIM_INFINITY;
  limit.rlim_cur=RLIM_INFINITY;
  int res2 = setrlimit(RLIMIT_MSGQUEUE,&limit);
  return res1 | res2;
}
int _init_rlimit=init_rlimit();
// }}}

unsigned int Channel_PMQ::ourQueueCounter=0;

Channel_PMQ::Channel_PMQ(const string &queue) // {{{
: myUnlink(false)
{
  if (queue=="")
  { // Create new uniqie name
    myName="/libpi-"+int2str(getpid())+"-"+int2str(++ourQueueCounter);
  }
  else if (queue[0]=='/')
  { // Use queue as name
    myName=queue;
  }
  else
  { // Create name based on queue
    myName="/libpi-static-"+queue;
  }
  cout << "mq_open(" << myName.c_str() << ",O_RDWR | O_CREAT, 0600, NULL)" << endl;
  myQueue=mq_open(myName.c_str(),O_RDWR | O_CREAT, 0600, NULL);
  cout << "mq_open(" << myName.c_str() << ",O_RDWR | O_CREAT, 0600, NULL)=" << myQueue << endl;
  if (myQueue==-1) // Error
    throw (string)"Unable to create message queue: " + myName + "\n"
                + "Error was: " + strerror(errno);
  mq_getattr(myQueue,&myAttributes);
} // }}}

Channel_PMQ::~Channel_PMQ() // {{{
{
  mq_close(myQueue);
  if (myUnlink)
    mq_unlink(myName.c_str());
} // }}}

void Channel_PMQ::Unlink() // {{{
{
  myUnlink=true;
} // }}}

void Channel_PMQ::Send(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " << " << msg.GetData() << endl;
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

void Channel_PMQ::Receive(Message &msg) // {{{
{
  int pos=0;
  int size;
  char *data = new char[myAttributes.mq_msgsize];
  int delta=mq_receive(myQueue, data, myAttributes.mq_msgsize,0); // Receive Size
  if (delta != sizeof(size))
    throw (string) "Channel_PMQ::Receive: Wrong header size";
  memcpy((char*)&size,data,sizeof(size));
  while (pos<size)
  { int delta=mq_receive(myQueue,data,myAttributes.mq_msgsize,0);
    if (delta==-1)
      throw (string) "Channel_PMQ::Receive: Unable to receive on message queue: " + myName + "\n"
                   + "Error was: " + strerror(errno);
    if (delta>size-pos)
      throw (string) "Channel_PMQ::Receive: Received too much data!" + "\n"
                   + "Remaining size: " + int2str(size-pos) + "\n"
                   + "Message size: " + int2str(delta);
    msg.AddData(data,delta);
    pos+=delta;
  }
  delete [] data;
  cout << GetAddress() << "(" << myQueue << ") " << " >> " << msg.GetData() << endl;
} // }}}

void Channel_PMQ::SingleSend(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " « " << msg.GetData() << endl;
  int size=msg.GetSize();
  if (sizeof(size)+size>myAttributes.mq_msgsize)
    throw (string)"Channel_PMQ::SingleSend: Meggage is too long for a single message";
  char *data = new char[myAttributes.mq_msgsize];
  memcpy(data,(const char*)&size,sizeof(size));
  memcpy(data+sizeof(size),msg.GetData(),size);
  mq_send(myQueue, data, size+4,0); // Send Size
  delete [] data;
} // }}}

void Channel_PMQ::SingleReceive(Message &msg) // {{{
{
  cout << GetAddress() << "(" << myQueue << ") " << " » ..." << endl;
  char *data = new char[myAttributes.mq_msgsize];
  mq_receive(myQueue, data, myAttributes.mq_msgsize,0);
  int size=*((int*)data);
  msg.AddData(data+sizeof(size),size);
  delete [] data;
  cout << GetAddress() << "(" << myQueue << ") " << " » " << msg.GetData() << endl;
} // }}}

string Channel_PMQ::GetAddress() // {{{
{ return myName;
} // }}}
