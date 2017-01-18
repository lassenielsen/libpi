#include <libpi/thread/channel.hpp>
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

void Channel::Send(shared_ptr<libpi::Value> val) // {{{
{ SingleSend(val);
} // }}}

void Channel::SingleSend(shared_ptr<libpi::Value> val) // {{{
{ lock.Lock();
  msgs.push(val);
  ++(msg_count);
  if (msg_count==1)
    sync.Release();
  lock.Release();
} // }}}

shared_ptr<libpi::Value> Channel::Receive() // {{{
{ return SingleReceive();
} // }}}

shared_ptr<libpi::Value> Channel::SingleReceive() // {{{
{ sync.Lock();
  lock.Lock();
  --(msg_count);
  shared_ptr<libpi::Value> result=msgs.front();
  msgs.pop();
  if (msg_count>0)
    sync.Release();

  lock.Release();
  return result;
} // }}}

std::string Channel::GetAddress() const // {{{
{ throw string("Error: libpi::thread::Channel is cannot be serialized.");
} // }}}

Channel &Channel::operator=(const Channel &rhs) // {{{
{ throw string("Error: libpi::thread::Channel cannot be copied.");
} // }}}

  }
}
