#pragma once
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: Bool class {{{
/*!
 * Bool implements boolean operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Bool : public Value // {{{
{ public:
    // Copy constructor and assignment
    Bool(const Bool &val);
    Bool &operator=(const Bool &rhs);

    // Constructors
    Bool(const std::string &str);
    Bool(bool val=false);
    virtual ~Bool();

    std::string GetType() const { return "boo"; }
    void ToString(std::ostream &dest) const;
    Bool operator&&(const Bool &rhs) const;
    Bool operator||(const Bool &rhs) const;
    Bool operator!() const;
    bool operator==(const Value &rhs) const;

    bool GetValue() const;

    static Value *ParseBool(std::istream &in);

  private:
    bool myValue;
}; // }}}

}
