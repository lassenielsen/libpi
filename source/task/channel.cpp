#include <libpi/task/channel.hpp>
#include <libpi/thread/channel.hpp>
#include <sstream>

using namespace std;
namespace libpi
{ namespace task
  {
Channel::Channel() // {{{
{ pthread_mutex_init(&myLock,NULL);
} // }}}

Channel::Channel(const Channel &rhs) // {{{
{ throw string("Error: Trying to copy a libpi::task::Channel object which cannot be cloned or copied");
} // }}}

Channel::~Channel() // {{{
{ pthread_mutex_destroy(&myLock);
} // }}}

void Channel::Unlink() // {{{
{
} // }}}

void Channel::Send(const shared_ptr<libpi::Value> &val) // {{{
{ SingleSend(val);
} // }}}

void Channel::SingleSend(const shared_ptr<libpi::Value> &val) // {{{
{ pthread_mutex_lock(&myLock);
  if (myTasks.size()>0) // Pop task
  { shared_ptr<Task> t=myTasks.front().first;
    shared_ptr<libpi::Value> &dst=myTasks.front().second;
    myTasks.pop();
    dst=val;
    pthread_mutex_unlock(&myLock);
    ++(*Task::ActiveTasks);
    Task::Tasks.Send(t);
  }
  else // Store in myMsgs
  { myMsgs.push(val);
    pthread_mutex_unlock(&myLock);
  }
} // }}}

shared_ptr<libpi::Value> Channel::Receive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

bool Channel::Receive(const shared_ptr<Task> &task, shared_ptr<libpi::Value> &dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

shared_ptr<libpi::Value> Channel::SingleReceive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

bool Channel::SingleReceive(const shared_ptr<Task> &task, shared_ptr<libpi::Value> &dest) // {{{
{ pthread_mutex_lock(&myLock);
  if (myMsgs.size()>0) // Pop msg
  { dest=myMsgs.front();
    myMsgs.pop();
    pthread_mutex_unlock(&myLock);
    return true;
  }
  else // Task waits in queue
  { myTasks.push(pair<shared_ptr<Task>,shared_ptr<libpi::Value>&>(task,dest));
    pthread_mutex_unlock(&myLock);
    return false;
  }
} // }}}

std::string Channel::GetAddress() const // {{{
{ throw string("Error: libpi::task::Channel is cannot be serialized.");
} // }}}

Channel &Channel::operator=(const Channel &rhs) // {{{
{ throw string("Error: libpi::task::Channel cannot be copied.");
} // }}}

  }
}
