#pragma once

#include <libpi/value.hpp>
#include <gmp.h>

namespace libpi
{

// DOCUMENTATION: IntValue class {{{
/*!
 * IntValue implements integer operation.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class IntValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    IntValue(const IntValue &val);
    IntValue &operator=(const IntValue &rhs);
    IntValue *Copy() const;

    // Constructors
    IntValue(const std::string &val);
    IntValue(mpz_t &val, bool clear_arg=false);
    IntValue(long val=0);
    virtual ~IntValue();

    std::string GetType() const { return "int"; }
    std::string ToString() const;
    IntValue operator+(const IntValue &rhs) const;
    IntValue operator-(const IntValue &rhs) const;
    IntValue operator*(const IntValue &rhs) const;
    IntValue operator/(const IntValue &rhs) const;
    bool operator<=(const IntValue &rhs) const;
    bool operator==(const Value &rhs) const;

    const mpz_t &GetValue() const;

    static Value *ParseInt(const std::string &str);
  private:
    mpz_t myValue;
}; // }}}

}
