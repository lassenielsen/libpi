#include <libpi/task/channel.hpp>
#include <libpi/thread/channel.hpp>
#include <sstream>

using namespace std;

extern libpi::thread::Channel _tasks;

namespace libpi
{ namespace task
  {
Channel::Channel() // {{{
: msgs()
, tasks()
, lock()
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
  if (tasks.size()>0) // Pop task
  { shared_ptr<Task> t=tasks.front().first;
    string dst=tasks.front().second;
    tasks.pop();
    t->Values()[dst]=val;
    _tasks.Send(t);
  }
  else
    msgs.push(val);
  lock.Release();
} // }}}

shared_ptr<libpi::Value> Channel::Receive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

shared_ptr<libpi::Value> Channel::Receive(shared_ptr<Task> task, string dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

shared_ptr<libpi::Value> Channel::SingleReceive() // {{{
{ throw string("Using original receive on task level channel");
}

shared_ptr<libpi::Value> Channel::SingleReceive(shared_ptr<Task> task, string dest) // {{{
{ shared_ptr<Value> result;
  lock.Lock();
  if (msgs.size()>0)
  { result=msgs.front();
    msgs.pop();
  }
  else
  { tasks.push(pair<shared_ptr<Task>,string>(task,dest));
    // NULL WILL BE RETURNED -- MUST BE HANDLED BY CALLER
  }
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
