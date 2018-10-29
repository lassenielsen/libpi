#pragma once
#include <libpi/task/task.hpp>
#include <libpi/thread/mutex.hpp>
#include <queue>
#include <atomic>
#include <unordered_set>

namespace libpi
{

  namespace task
  {
// DOCUMENTATION: Worker class {{{
/*! Abstract declaration of the framework for executing tasks
 */
// }}}
    class Worker : public libpi::Value, public libpi::gc::GCRegistrant // {{{
    { public:
        Worker() : Value(NULL) {}
        virtual ~Worker();

        virtual void Work()=0; // Perform tasks
        //! EmplyTask is used to assign a task to an idle worker
        virtual void EmployTask(Task *task)=0;
        //! AddTask is used to add a new or previously inactive task to the queue
        virtual void AddTask(Task *task)=0;
        //! QueueTask is used to push an already active task on the queue
        virtual void QueueTask(Task *task)=0;
        //! GCRegister registers an address to be collected when no longer referenced
        virtual void GCRegister(void *object)=0;
        virtual const std::unordered_set<void*> &GCMarks() const=0;   //!< Used by GC to access marks
        virtual const std::unordered_set<void*> &GCValues() const=0;  //!< Used by GC to access potential sweeps
        virtual void GCTask()=0; //!< Used by GC to order the calculation of GCMarks and GCValues
        virtual void GCWait()=0; //!< Used by GC to wait until GCMarks and GCValues are available

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
        void EmployTask(Task *task);
        void AddTask(Task *task);
        void QueueTask(Task *task);
        const std::queue<Task*> &GetActiveTasks() { return myActiveTasks; }
        // Garbage Collection functionality
        /*! Adds @object to set of known GC managed objects.
            After each collection GCValues is reduced to the set of marks,
            since the unmarked values are either collected or present in the
            marks of other workers. */
        void GCRegister(void *object) { myGCNewValues.insert(object); }
        const std::unordered_set<void*> &GCMarks() const { return myGCMarks; }
        const std::unordered_set<void*> &GCValues() const { return myGCValues; }
        void GCTask() { myGCFlag=true; }
        void GCWait() { myGCLock.Lock(); }

      private:
        std::queue<Task*> myActiveTasks;
        libpi::thread::Mutex myWaitLock;

	std::unordered_set<void*> myGCMarks;
        std::unordered_set<void*> myGCValues;
        std::unordered_set<void*> myGCNewValues;
        bool myGCFlag;
        libpi::thread::Mutex myGCLock;

        static std::queue<Worker*> ourIdleWorkers; //! Queue of workers in the pool without tasks
        static std::atomic<size_t> ourIdleWorkersSize; //! Aggregating the size of ourIdleWorkers
        static libpi::thread::Mutex ourIdleWorkersLock; //! Lock for ourIdleWorkers
    }; // }}}
  }
}
