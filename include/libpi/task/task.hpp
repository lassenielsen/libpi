#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <libpi/value.hpp>
#include <vector>
#include <atomic>
#include <libpi/thread/channel.hpp>

namespace libpi
{

  namespace task
  {
    typedef std::vector<std::shared_ptr<Value> > Closure;

// DOCUMENTATION: Task class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
    class Task : public libpi::Value // {{{
    { public:
        Task() {}
        virtual ~Task();
        virtual std::string GetType() const;
        virtual void ToStream(std::ostream &dest) const;
    
        void *GetLabel() {return myLabel;}
        void SetLabel(void *label) {myLabel=label;}
        Closure &Values() {return myValues;}
        size_t &GetSteps() {return mySteps;}

        static libpi::thread::Channel Tasks;     //! Task queue for worker threads
        static std::atomic<size_t> *ActiveTasks; //! Actual number of active processes
        static size_t TargetTasks;               //! Desired number of active processes - defaults to number of cpu-cores
      private:
        void *myLabel;
        Closure myValues;
        size_t mySteps;
    }; // }}}

    // Define Task-level Events
    class TaskEvent {};
    
    class TaskPauseEvent: public TaskEvent
    {
    };
  }
}
