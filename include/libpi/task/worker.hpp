#pragma once
#include <libpi/task/task.hpp>
#include <libpi/thread/mutex.hpp>
#include <queue>
#include <atomic>

namespace libpi
{

  namespace task
  {
// DOCUMENTATION: Worker class {{{
/*! Abstract declaration of the framework for executing tasks
 */
// }}}
    class Worker : public libpi::Value // {{{
    { public:
        Worker() {}
	virtual ~Worker();

        virtual void Work()=0; // Perform tasks
	//! EmplyTask is used to assign a task to an idle task
        virtual void EmployTask(std::shared_ptr<Task> &task)=0;
	//! AddTask is used to add a new or previously inactive task to the queue
        virtual void AddTask(std::shared_ptr<Task> &task)=0;
	//! QueueTask is used to push an already active task on the queue
        virtual void QueueTask(std::shared_ptr<Task> &task)=0;

        static std::atomic<size_t> ActiveTasks;  //! Actual number of active processes
        static size_t TargetTasks;               //! Desired number of active processes - defaults to number of cpu-cores
        static size_t Workers;                   //! Desired number of worker threads - defaults to number of cpu-cores
    }; // }}}
// DOCUMENTATION: Worker_Pool class {{{
/*! Executin tasks using a thread pool
    When queueing task:
    If @myActiveTasks is empty, commence task immediately
    Otherwise if @ourIdleWorkersSize>0 lock pop and unloc and assign task to idle worker
    Oherwize push task in myActiveTasks for scheduling, and resume next task
 */
// }}}
    class Worker_Pool : public Worker // {{{
    { public:
        Worker_Pool();
	virtual ~Worker_Pool();

        void Work();
        void EmployTask(std::shared_ptr<Task> &task);
        void AddTask(std::shared_ptr<Task> &task);
        void QueueTask(std::shared_ptr<Task> &task);
        const std::queue<std::shared_ptr<Task> > &GetActiveTasks() { return myActiveTasks; }


      private:
        std::queue<std::shared_ptr<Task> > myActiveTasks;
        static std::queue<Worker*> ourIdleWorkers; //! Queue of workers in the pool without tasks
        static std::atomic<size_t> ourIdleWorkersSize; //! Aggregating the size of ourIdleWorkers
        static libpi::thread::Mutex ourIdleWorkersLock; //! Lock for ourIdleWorkers
        libpi::thread::Mutex myWaitLock;
    }; // }}}
  }
}
