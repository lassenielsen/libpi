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
    std::shared_ptr<Bool> operator&&(const Bool &rhs) const;
    //! Boolean disjunction
    std::shared_ptr<Bool> operator||(const Bool &rhs) const;
    //! Boolean negation
    std::shared_ptr<Bool> operator!() const;
    std::shared_ptr<Bool> operator==(const Value &rhs) const;
    std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    std::shared_ptr<Bool> operator<(const Value &rhs) const;
    std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    std::shared_ptr<Bool> operator>(const Value &rhs) const;

    bool GetValue() const { return myValue; }

    static Value *ParseBool(std::istream &in);
    static std::shared_ptr<Bool> GetInstance(bool val) // {{{
    { if (val)
        return trueInstance;
      else
        return falseInstance;
    } // }}}
    static std::shared_ptr<Bool> GetInstance(const std::string &val) // {{{
    { if (val=="true")
        return trueInstance;
      else if (val=="false")
        return falseInstance;
      else throw (std::string)"Bool::GetInstance: Bad message value: " + val;
    } // }}}

  private:
    bool myValue;
    static std::shared_ptr<Bool> trueInstance;
    static std::shared_ptr<Bool> falseInstance;
}; // }}}

}
