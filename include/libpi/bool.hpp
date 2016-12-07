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
    Bool(const std::string &str);
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
    bool operator==(const Value &rhs) const;

    bool GetValue() const;

    static Value *ParseBool(std::istream &in);

  private:
    bool myValue;
}; // }}}

}
