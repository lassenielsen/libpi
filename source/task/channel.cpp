#include <libpi/task/channel.hpp>
#include <libpi/task/worker.hpp>
#include <libpi/task/channel.hpp>
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
  while (!myTasks.empty())
  { myTasks.front().first->RemoveRef();
    (*myTasks.front().second)->RemoveRef();
    myTasks.pop();
  }
  while (!myIntTasks.empty())
  { myIntTasks.front().first->RemoveRef();
    myIntTasks.pop();
  }
  while (!myMsgs.empty())
  { myMsgs.front()->RemoveRef();
    myMsgs.pop();
  }
  //Not included, because not necessary, and inefficient
  //while (!myIntMsgs.empty())
  //{ myIntMsgs.pop();
  //}
} // }}}

void Channel::Unlink() // {{{
{
} // }}}

void Channel::Send(libpi::Value *val) // {{{
{ throw string("Using original send on task level channel");
} // }}}

void Channel::SingleSend(libpi::Value *val) // {{{
{ throw string("Using original send on task level channel");
} // }}}

void Channel::Send(Task *sender, libpi::Value *val) // {{{
{ SingleSend(sender,val);
} // }}}

void Channel::SingleSend(Task *sender, libpi::Value *val) // {{{
{ pthread_mutex_lock(&myLock);
  if (!myTasks.empty()) // Pop task
  { pair<Task*,libpi::Value**> elt=myTasks.front();
    myTasks.pop();
    pthread_mutex_unlock(&myLock);
    AssignValue(elt.second,val);
    elt.first->SetWorker(&sender->GetWorker()); // Use senders worker, to avoid the need for synchronization
    sender->GetWorker().AddTask(elt.first);
  }
  else // Store in myMsgs
  { val->AddRef();
    myMsgs.push(val);
    pthread_mutex_unlock(&myLock);
  }
} // }}}

void Channel::Send(Task *sender, long int msg) // {{{
{ SingleSend(sender,msg);
} // }}}

void Channel::SingleSend(Task *sender, long int msg) // {{{
{ pthread_mutex_lock(&myLock);
  if (!myIntTasks.empty()) // Pop task
  { pair<Task*,long int*> elt=myIntTasks.front();
    myIntTasks.pop();
    pthread_mutex_unlock(&myLock);
    *elt.second=msg;
    elt.first->SetWorker(&sender->GetWorker()); // Use senders worker, to avoid the need for synchronization
    sender->GetWorker().AddTask(elt.first);
  }
  else // Store in myMsgs
  { myIntMsgs.push(msg);
    pthread_mutex_unlock(&myLock);
  }
} // }}}

libpi::Value *Channel::Receive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

libpi::Value *Channel::SingleReceive() // {{{
{ throw string("Using original receive on task level channel");
} // }}}

bool Channel::Receive(Task *task, libpi::Value **dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

bool Channel::SingleReceive(Task *task, libpi::Value **dest) // {{{
{ pthread_mutex_lock(&myLock);
  if (!myMsgs.empty()) // Pop msg
  { Value *msg=myMsgs.front();
    myMsgs.pop();
    pthread_mutex_unlock(&myLock);
    AssignValue(dest,msg);
    msg->RemoveRef();
    return true;
  }
  else // Task waits in queue
  { myTasks.push(pair<Task*,libpi::Value**>(task,dest));
    pthread_mutex_unlock(&myLock);
    return false;
  }
} // }}}

bool Channel::Receive(Task *task, long int *dest) // {{{
{ return SingleReceive(task,dest);
} // }}}

bool Channel::SingleReceive(Task *task, long int *dest) // {{{
{ pthread_mutex_lock(&myLock);
  if (!myIntMsgs.empty()) // Pop msg
  { *dest=myIntMsgs.front();
    myIntMsgs.pop();
    pthread_mutex_unlock(&myLock);
    return true;
  }
  else // Task waits in queue
  { myIntTasks.push(pair<Task*,long int*>(task,dest));
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
