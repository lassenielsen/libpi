#include <libpi/thread/channel.hpp>
#include <libpi/task/task.hpp>
#include <libpi/task/worker.hpp>
#include <sstream>

using namespace std;

namespace libpi
{ namespace thread
  {
Channel::Channel(libpi::gc::GCRegistrant *registrant) // {{{
: libpi::Channel(registrant)
, msgs()
, sync(true)
, lock()
, msg_count(0)
{
} // }}}

Channel::Channel(const Channel &rhs) // {{{
: libpi::Channel(NULL)
{ throw string("Error: Trying to copy a libpi::thread::Channel object which cannot be cloned or copied");
} // }}}

Channel::~Channel() // {{{
{
} // }}}

void Channel::Unlink() // {{{
{
} // }}}

void Channel::Send(libpi::Value *val) // {{{
{ SingleSend(val);
} // }}}

void Channel::SingleSend(libpi::Value *val) // {{{
{ lock.Lock();
  msgs.push(val);
  int currentCount=++(msg_count);
  lock.Release();
  if (currentCount==1)
    sync.Release();
} // }}}

void Channel::Send(const task::Task *sender, libpi::Value *val) // {{{
{ SingleSend(sender,val);
} // }}}

void Channel::SingleSend(const task::Task *sender, libpi::Value *val) // {{{
{ SingleSend(val);
} // }}}

libpi::Value *Channel::Receive(libpi::gc::GCRegistrant *registrant) // {{{
{ return SingleReceive(registrant);
} // }}}

bool Channel::Receive(task::Task *receiver, libpi::Value *&dest) // {{{
{ return SingleReceive(receiver,dest);
} // }}}

libpi::Value *Channel::SingleReceive(libpi::gc::GCRegistrant *registrant) // {{{
{ sync.Lock();
  lock.Lock();
  size_t currentCount=--(msg_count);
  libpi::Value *result=msgs.front();
  msgs.pop();
  if (currentCount>0)
    sync.Release();
  lock.Release();
  return result;
} // }}}

bool Channel::SingleReceive(task::Task *receiver, libpi::Value *&dest) // {{{
{ dest=SingleReceive(&(receiver->GetWorker()));
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
