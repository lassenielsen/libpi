#include <libpi/thread/channel.hpp>
#include <libpi/task/task.hpp>
#include <sstream>

using namespace std;

namespace libpi
{ namespace thread
  {
Channel::Channel() // {{{
: msgs()
, sync(true)
, lock()
, msg_count(0)
{
} // }}}

Channel::Channel(const Channel &rhs) // {{{
{ throw string("Error: Trying to copy a libpi::thread::Channel object which cannot be cloned or copied");
} // }}}

Channel::~Channel() // {{{
{
} // }}}

void Channel::Unlink() // {{{
{
} // }}}

void Channel::Send(const libpi::Value *val) // {{{
{ SingleSend(val);
} // }}}

void Channel::SingleSend(const libpi::Value *val) // {{{
{ lock.Lock();
  msgs.push(val);
  int currentCount=++(msg_count);
  lock.Release();
  if (currentCount==1)
    sync.Release();
} // }}}

void Channel::Send(const task::Task *sender, const libpi::Value *val) // {{{
{ SingleSend(sender,val);
} // }}}

void Channel::SingleSend(const task::Task *sender, const libpi::Value *val) // {{{
{ SingleSend(val);
} // }}}

const libpi::Value *Channel::Receive() // {{{
{ return SingleReceive();
} // }}}

bool Channel::Receive(const task::Task *receiver, const libpi::Value *&dest) // {{{
{ return SingleReceive(receiver,dest);
} // }}}

const libpi::Value *Channel::SingleReceive() // {{{
{ sync.Lock();
  lock.Lock();
  size_t currentCount=--(msg_count);
  const libpi::Value *result=msgs.front();
  msgs.pop();
  if (currentCount>0)
    sync.Release();
  lock.Release();
  return result;
} // }}}

bool Channel::SingleReceive(const task::Task *receiver, const libpi::Value *&dest) // {{{
{ dest=SingleReceive();
  return true;
} // }}}

std::string Channel::GetAddress() const // {{{
{ throw string("Error: libpi::thread::Channel is cannot be serialized.");
} // }}}

Channel &Channel::operator=(const Channel &rhs) // {{{
{ throw string("Error: libpi::thread::Channel cannot be copied.");
} // }}}

  }
}
