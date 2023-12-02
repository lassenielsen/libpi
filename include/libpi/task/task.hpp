#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <libpi/value.hpp>
#include <vector>
#include <atomic>

namespace libpi
{

  namespace task
  {
    typedef std::vector<Value*> Closure;
    class Worker;
// DOCUMENTATION: Task class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
    class Task : public libpi::Value // {{{
    { public:
        Task() { tmp=NULL; }
        virtual ~Task();
        virtual std::string GetType() const;
        virtual void ToStream(std::ostream &dest) const;
    
        void *GetLabel() {return myLabel;}
        void SetLabel(void *label) {myLabel=label;}
        Worker &GetWorker() {return *myWorker;}
        void SetWorker(Worker *worker) {myWorker=worker;}

        static size_t MaxSteps;                  //! Maximum number of steps before yielding

        long int tmpInt;                         //! A temporary value that can be used by the task for example to receive a boolean value
        libpi::Value *tmp;                       //! A temporary value that can be used by the task for example to receive a value
        std::vector<libpi::Value*> tmps;         //! A list of temporary values that can be used by the task, used mainly for linking

      private:
        void *myLabel;
        Worker *myWorker;
    }; // }}}
  }
}
