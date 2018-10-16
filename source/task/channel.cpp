#include <libpi/task/channel.hpp>
#include <libpi/task/worker.hpp>
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

void Channel::Send(const libpi::Value *val) // {{{
{ throw string("Using original send on task level channel");
} // }}}

void Channel::SingleSend(const libpi::Value *val) // {{{
{ throw string("Using original send on task level channel");
} // }}}

void Channel::Send(const Task *sender, const libpi::Value *val) // {{{
{ SingleSend(sender,val);
} // }}}

void Channel::SingleSend(const Task *sender, const libpi::Value *val) // {{{
{ pthread_mutex_lock(&myLock);
  if (myTasks.size()>0) // Pop task
  { pair<Task*,libpi::Value*> elt=myTasks.front();
    myTasks.pop();
    pthread_mutex_unlock(&myLock);
    elt.second=val;
    elt.first->SetWorker(&sender->GetWorker()); // Use senders worker, to avoid the need for synchronization
    sender->GetWorker().AddTask(elt.first);
  }
  else // Store in myMsgs
  { myMsgs.push(val);
    pthread_mutex_unlock(&myLock);
  }
} // }}}

libpi::Value *Channel::Receive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

bool Channel::Receive(const Task *task, libpi::Value *dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

libpi::Value *Channel::SingleReceive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

bool Channel::SingleReceive(const Task *task, libpi::Value *dest) // {{{
{ pthread_mutex_lock(&myLock);
  if (myMsgs.size()>0) // Pop msg
  { dest=myMsgs.front();
    myMsgs.pop();
    pthread_mutex_unlock(&myLock);
    return true;
  }
  else // Task waits in queue
  { myTasks.push(pair<Task*,libpi::Value*>(task,dest));
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

void Channel::Mark(unordered_set<void*> &marks) const // {{{
{ if (marks.member((void*)this))
    return; // Nothing new to add
    
  marks.insert((void*)this);
  for (queue<pair<Task*,Value*> >::const_iterator tsk=myTasks.begin(); tsk!=myTasks.end(); ++tsk)
    tsk->Mark(marks);
  for (queue<Value*>::const_iterator val=myMsgs.begin(); val!=myMsgs.end(); ++val)
    val->Mark(marks);
} // }}}
  }
}
