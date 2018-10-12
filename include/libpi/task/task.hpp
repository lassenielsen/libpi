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
    class Worker;
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
        Worker &GetWorker() {return *myWorker;}
        void SetWorker(Worker *worker) {myWorker=worker;}

        static size_t MaxSteps;                  //! Maximum number of steps before yielding

        std::shared_ptr<libpi::Value> tmp;       //! A temporary value that can be used by the task
        std::vector<std::shared_ptr<libpi::Value> > tmps;       //! A list of temporary values that can be used by the task, used mainly for linking

      private:
        void *myLabel;
        Worker *myWorker;
    }; // }}}
  }
}
