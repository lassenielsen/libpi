#include <libpi/thread/channel.hpp>
#include <sstream>

using namespace std;

namespace libpi
{ namespace thread
  {
Channel::Channel() // {{{
: msgs(new std::queue<libpi::Value*>())
, sync(new Mutex(true))
, lock(new Mutex())
, msg_count(new atomic<int>(0))
, ref_count(new atomic<int>(1))
{
} // }}}

Channel::Channel(const Channel &rhs) // {{{
: msgs(rhs.msgs)
, sync(rhs.sync)
, lock(rhs.lock)
, msg_count(rhs.msg_count)
, ref_count(rhs.ref_count)
{ lock->Lock();
  ++(*ref_count);
  lock->Release();
} // }}}

Channel::~Channel() // {{{
{ Detach();
} // }}}

Channel *Channel::Copy() const // {{{
{ return new Channel(*this);
} // }}}

void Channel::Unlink() // {{{
{
} // }}}

void Channel::Send(libpi::Value *msg) // {{{
{ lock->Lock();
  ++(*msg_count);
  msgs->push(msg);
  if (*msg_count<=0)
    sync->Release();
  lock->Release();
} // }}}

void Channel::SingleSend(libpi::Value *msg) // {{{
{ Send(msg);
} // }}}

libpi::Value *Channel::Receive() // {{{
{ lock->Lock();
  --(*msg_count);
  if (*msg_count<0)
  { lock->Release();
    sync->Lock();
    lock->Lock();
  }

  libpi::Value *result=msgs->front();
  msgs->pop();
  lock->Release();
  return result;
} // }}}

libpi::Value *Channel::SingleReceive() // {{{
{ return Receive();
} // }}}

std::string Channel::GetAddress() const // {{{
{ stringstream ss;
  ss << this;
  return ss.str();
} // }}}

Channel &Channel::operator=(const Channel &rhs) // {{{
{ Detach();
  rhs.lock->Lock();
  ref_count=rhs.ref_count;
  ++(*ref_count);
  msgs=rhs.msgs;
  msg_count=rhs.msg_count;
  sync=rhs.sync;
  lock=rhs.lock;
  rhs.lock->Release();
  return *this;
} // }}}

void Channel::Detach() // {{{
{ lock->Lock();
  --(*ref_count);
  if (*ref_count<=0)
  { delete ref_count;
    delete msg_count;
    sync->Release();
    delete sync;
    while (msgs->size()>0)
    { //std::cout << "Deleting msg: " << msgs->front()->ToString() << std::endl;
      delete msgs->front();
      msgs->pop();
    }
    delete msgs;
    lock->Release();
    delete lock;
  }
  else
    lock->Release();

  ref_count=NULL;
  msg_count=NULL;
  lock=NULL;
  sync=NULL;
  msgs=NULL;
} // }}}

  }
}
