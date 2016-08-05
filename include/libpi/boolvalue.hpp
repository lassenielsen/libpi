#pragma once
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: BoolValue class {{{
/*!
 * BoolValue implements boolean operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class BoolValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    BoolValue(const BoolValue &val);
    BoolValue &operator=(const BoolValue &rhs);
    BoolValue *Copy() const;

    // Constructors
    BoolValue(const std::string &str);
    BoolValue(bool val=false);
    virtual ~BoolValue();

    std::string GetType() const { return "boo"; }
    std::string ToString() const;
    BoolValue operator&&(const BoolValue &rhs) const;
    BoolValue operator||(const BoolValue &rhs) const;
    BoolValue operator!() const;
    bool operator==(const Value &rhs) const;

    bool GetValue() const;

    static Value *ParseBool(const std::string &str);

  private:
    bool myValue;
}; // }}}

}
