#include <libpi/task/worker.hpp>
#include<thread>

using namespace std;
using namespace libpi::task;

bool _methods(shared_ptr<Task> &_task);

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
{
} // }}}

Worker_Pool::~Worker_Pool() // {{{
{
} // }}}

/*void Worker_Pool::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Test if program is complete
      if (ActiveTasks==0)
      { ourIdleWorkersLock.Lock();
        shared_ptr<Task> nullTask;
	while (!ourIdleWorkers.empty())
        { ourIdleWorkers.front()->EmployTask(nullTask);
          ourIdleWorkers.pop();
        }
        ourIdleWorkersSize=0;
        ourIdleWorkersLock.Release();
        break;
      }
      // Find next task
      while (myActiveTasks.empty())
      { ourIdleWorkersLock.Lock();
        ourIdleWorkers.push(this);
        ++ourIdleWorkersSize;
        ourIdleWorkersLock.Release();
        myWaitLock.Lock();
      }
      shared_ptr<Task> task=myActiveTasks.front();
      myActiveTasks.pop();

      resume_task:
      if (!task)
        break;
      if ((&task->GetWorker())!=this)
        cerr << "Running task with different worker!!!" << endl;
      if (_methods(task))
      { if (myActiveTasks.empty())
          goto resume_task;
        else QueueTask(task);
      }
      else
        --ActiveTasks;
    }
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
  }
  return;
} // }}}
*/

void Worker_Pool::EmployTask(shared_ptr<Task> &task) // {{{
{ if (task)
    task->SetWorker(this);
  myActiveTasks.push(task);
  myWaitLock.Release();
} // }}}

void Worker_Pool::AddTask(shared_ptr<Task> &task) // {{{
{ ++ActiveTasks;
  QueueTask(task);
} // }}}

void Worker_Pool::QueueTask(shared_ptr<Task> &task) // {{{
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
