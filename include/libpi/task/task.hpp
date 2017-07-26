#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <libpi/value.hpp>
#include <unordered_map>

namespace libpi
{

  namespace task
  {
    typedef std::unordered_map<std::string,std::shared_ptr<Value> > Closure;

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
  
      private:
        void *myLabel;
        Closure myValues;
        size_t mySteps;
    }; // }}}

  }
}
