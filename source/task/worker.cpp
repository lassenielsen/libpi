#include <libpi/task/worker.hpp>
#include<thread>

using namespace std;
using namespace libpi::task;

bool _methods(Task *_task);

atomic<size_t> Worker::ActiveTasks(0);
size_t Worker::TargetTasks=std::thread::hardware_concurrency(); // Target number of active processes per worker
size_t Worker::Workers=std::thread::hardware_concurrency(); // Target number of active processes

Worker::~Worker() // {{{
{
} // }}}

queue<Worker*> Worker_Pool::ourIdleWorkers;
atomic<size_t> Worker_Pool::ourIdleWorkersSize;
libpi::thread::Mutex Worker_Pool::ourIdleWorkersLock;

Worker_Pool::Worker_Pool() // {{{
: myWaitLock(true)
, myGCLock(true)
, myGCFlag(false)
{
} // }}}

Worker_Pool::~Worker_Pool() // {{{
{
} // }}}

void Worker_Pool::EmployTask(Task *task) // {{{
{ if (task)
    task->SetWorker(this);
  myActiveTasks.push(task);
  myWaitLock.Release();
} // }}}

void Worker_Pool::AddTask(Task *task) // {{{
{ ++ActiveTasks;
  QueueTask(task);
} // }}}

void Worker_Pool::QueueTask(Task *task) // {{{
{ if (ourIdleWorkersSize>0)
  { ourIdleWorkersLock.Lock();
    if (ourIdleWorkersSize>0)
    { --ourIdleWorkersSize;
      ourIdleWorkers.front()->EmployTask(task);
      ourIdleWorkers.pop();
      ourIdleWorkersLock.Release();
      return;
    }
    else
      ourIdleWorkersLock.Release();
  }
  myActiveTasks.push(task);
} // }}}
