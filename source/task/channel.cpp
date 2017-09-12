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

void Channel::Send(shared_ptr<libpi::Value> val) // {{{
{ SingleSend(val);
} // }}}

void Channel::SingleSend(shared_ptr<libpi::Value> val) // {{{
{ pthread_mutex_lock(&myLock);
  if (myTasks.size()>0) // Pop task
  { shared_ptr<Task> t=myTasks.front().first;
    string dst=myTasks.front().second;
    myTasks.pop();
    t->Values()[dst]=val;
    pthread_mutex_lock(&myLock);
    throw TaskResumeEvent(t);
  }
  else // Store in myMsgs
  { myMsgs.push(val);
    pthread_mutex_lock(&myLock);
  }
} // }}}

shared_ptr<libpi::Value> Channel::Receive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

void Channel::Receive(shared_ptr<Task> task, string dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

shared_ptr<libpi::Value> Channel::SingleReceive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

void Channel::SingleReceive(shared_ptr<Task> task, string dest) // {{{
{ pthread_mutex_lock(&myLock);
  if (myMsgs.size()>0) // Pop msg
  { task->Values()[dest]=myMsgs.front();
    myMsgs.pop();
    pthread_mutex_unlock(&myLock);
  }
  else // Task waits in queue
  { myTasks.push(pair<shared_ptr<Task>,std::string>(task,dest));
    pthread_mutex_unlock(&myLock);
    throw TaskPauseEvent();
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