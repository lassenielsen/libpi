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
#include <unordered_set>

namespace libpi
{

  namespace task
  {
    class Worker;
// DOCUMENTATION: Task class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
    class Task : public libpi::Value // {{{
    { public:
        Task(libpi::gc::GCRegistrant *registrant) : Value(NULL) /* NO GC OF TAASKS! */, tmp(NULL) {}
        virtual ~Task();
        virtual std::string GetType() const;
        virtual void ToStream(std::ostream &dest) const;
        virtual std::string GetName() const;
    
        void *GetLabel() {return myLabel;}
        void SetLabel(void *label) {myLabel=label;}
        Worker &GetWorker() const {return *myWorker;}
        void SetWorker(Worker *worker) {myWorker=worker;}

        static size_t MaxSteps;                           //! Maximum number of steps before yielding

        libpi::Value *tmp;                                //! A temporary value that can be used by the task
        std::vector<libpi::Value*> tmps;                  //! A list of temporary values that can be used by the task, used mainly for linking

        virtual void Mark(std::unordered_set<Value*> &marks);   //! Mark all live values for garbage collection

      private:
        void *myLabel;
        Worker *myWorker;
    }; // }}}
  }
}
