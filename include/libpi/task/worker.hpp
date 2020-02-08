#pragma once
#include <libpi/task/task.hpp>
#include <libpi/thread/mutex.hpp>
#include <list>
#include <queue>
#include <set>
#include <atomic>
#include <unordered_set>

namespace libpi
{

  namespace task
  {
// DOCUMENTATION: Worker class {{{
/*! A worker is a framework for executing tasks with support for distribution
 *  over threads but potentially also over clusters.
 *  The framework implements distributed scheduling, sleeping, and distributed
 *  garbage collection.
 *
 *  Current implementation Executin tasks using a thread pool
 *
 *  When queueing task:
 *  If @myActiveTasks is empty, commence task immediately
 *  Otherwise if @ourIdleWorkersSize>0 lock pop and unloc and assign task to idle worker
 *  Oherwize push task in myActiveTasks for scheduling, and resume next task
 *
 *  When garbage collecting:
 *  Perform marking, store results in myGCValues and myGCMarks, and add worker to ourGCReady set.
 *  When all workers are in ourGCReady, the master thread will collect all
 *  values and marks, and delete the values that have no been marked by any
 *  worker.
 */
// }}}
    class Worker : public libpi::Value, public libpi::gc::GCRegistrant // {{{
    { public:
        Worker();
        virtual ~Worker();

        void Work(); //!< Main loop, scheduling tasks
        //! Activate is used to release the myWaitLock. This is used when garbage collecting, while waiting for active tasks.
        void Activate() { myWaitLock.Release(); }
        //! EmplyTask is used when assigning a task to an idle worker
        void EmployTask(Task *task) // {{{
        { if (task)
            task->SetWorker(this);
          myActiveTasks.push_back(task);
          Activate();
        } // }}}
        //! AddTask is used to add a new or previously inactive task to the queue
        void AddTask(Task *task) // {{{
        { ++ActiveTasks;
          QueueTask(task);
        } // }}}
        //! QueueTask is used to push an already active task on the queue
        void QueueTask(Task *task) // {{{
        { if (ourIdleWorkersSize>0)
          { ourIdleWorkersLock.Lock();
            if (ourIdleWorkersSize>0)
            { --ourIdleWorkersSize;
              Worker *iworker=ourIdleWorkers.front();
              ourIdleWorkers.pop_front();
              ourIdleWorkersLock.Release();
              task->Mark(myGCNewMarks);
              iworker->EmployTask(task);
              return;
            }
            else
              ourIdleWorkersLock.Release();
          }
          myActiveTasks.push_back(task);
        } // }}}
        const std::list<Task*> &GetActiveTasks() const { return myActiveTasks; }
        std::list<Task*> &GetActiveTasks() { return myActiveTasks; }
        // Garbage Collection functionality
        /*! Adds @object to set of known GC managed objects.
            After each collection GCValues is reduced to the set of marks,
            since the unmarked values are either collected or present in the
            marks of other workers. */
        void GCRegister(libpi::Value *object) { myGCNewValues.insert(object); }
        //! Used by GC to access marks
        const std::unordered_set<libpi::Value*> &GCMarks() const { return myGCMarks; }
        //! Used by GC to access potential sweeps
        const std::unordered_set<libpi::Value*> &GCValues() const { return myGCValues; }
        //! Used by GC to order the calculation of GCMarks and GCValues
        void GCTask() { myGCReady=false; myGCFlag=true; }
        //! Used by GC to determine if GC marks and values are ready for collection
        bool GCReady() { return myGCReady; }
        //! Used by GC to signal that GC is complete, and it is safe to resume work
        void GCDone() { myGCReady=false; return myGCDoneLock.Release(); }
        //! Used by tasks to pre-mark values
        void GCMark(libpi::Value *object) { if (object) object->Mark(myGCNewMarks); }
        //! Used by tasks to clear pre-marked values
        void GCClearMarks() { myGCNewMarks.clear(); }

        //! Actual number of active processes
        static std::atomic<size_t> ActiveTasks;
        //! Desired number of active processes - defaults to number of cpu-cores
        static size_t TargetTasks;
        //! Desired number of worker threads - defaults to number of cpu-cores
        static size_t Workers;

        static std::list<Worker*> &GetIdleWorkers() {return ourIdleWorkers;}
        static std::atomic<size_t> &GetIdleWorkersSize() {return ourIdleWorkersSize;}
        static libpi::thread::Mutex &GetIdleWorkersLock() {return ourIdleWorkersLock;}

      private:
        std::list<Task*> myActiveTasks;
        libpi::thread::Mutex myWaitLock;

        libpi::thread::Mutex myGCDoneLock;
        std::unordered_set<libpi::Value*> myGCMarks;
        std::unordered_set<libpi::Value*> myGCNewMarks;
        std::unordered_set<libpi::Value*> myGCValues;
        std::unordered_set<libpi::Value*> myGCNewValues;
        bool myGCFlag;
        bool myGCReady;

        static std::list<Worker*> ourIdleWorkers; //!< Queue of workers in the pool without tasks
        static std::atomic<size_t> ourIdleWorkersSize; //!< Aggregating the size of ourIdleWorkers
        static libpi::thread::Mutex ourIdleWorkersLock; //!< Lock for ourIdleWorkers
    }; // }}}
  }
}
