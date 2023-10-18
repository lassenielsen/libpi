#pragma once
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: Bool class {{{
/*!
 * Bool implements a boolean value and the possible operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Bool : public Value // {{{
{ public:
    // Copy constructor and assignment
    Bool(const Bool &val);

    // Constructors
    Bool(bool val=false);
    virtual ~Bool();

    std::string GetType() const { return "boo"; }
    void ToStream(std::ostream &dest) const;
    //! Boolean conjunction
    Bool* operator&&(const Bool &rhs) const;
    //! Boolean disjunction
    Bool* operator||(const Bool &rhs) const;
    //! Boolean negation
    Bool* operator!() const;
    Bool* operator==(const Value &rhs) const;
    Bool* operator<=(const Value &rhs) const;
    Bool* operator<(const Value &rhs) const;
    Bool* operator>=(const Value &rhs) const;
    Bool* operator>(const Value &rhs) const;

    bool GetValue() const { return myValue; }

    static Value* ParseBool(std::istream &in);
    static Bool* GetInstance(bool val) // {{{
    { if (val)
      { trueInstance->AddRef();
        return trueInstance;
      }
      else
      { falseInstance->AddRef();
        return falseInstance;
      }
    } // }}}
    static Bool *GetInstance(const std::string &val) // {{{
    { if (val=="true")
      { trueInstance->AddRef();
        return trueInstance;
      }
      else if (val=="false")
      { falseInstance->AddRef();
        return falseInstance;
      }
      else throw (std::string)"Bool::GetInstance: Bad message value: " + val;
    } // }}}

  private:
    bool myValue;
    static Bool* trueInstance;
    static Bool* falseInstance;
}; // }}}

}
