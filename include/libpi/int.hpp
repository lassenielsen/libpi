#pragma once

#include <libpi/value.hpp>
#include <gmp.h>

namespace libpi
{

// DOCUMENTATION: Int class {{{
/*!
 * Int implements integer operation.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Int : public Value // {{{
{ public:
    // Copy constructor and assignment
    Int(const Int &val);
    Int &operator=(const Int &rhs);

    // Constructors
    Int(const std::string &val);
    Int(mpz_t &val, bool clear_arg=false);
    Int(long val=0);
    virtual ~Int();

    std::string GetType() const { return "int"; }
    std::string ToString() const;
    Int operator+(const Int &rhs) const;
    Int operator-(const Int &rhs) const;
    Int operator*(const Int &rhs) const;
    Int operator/(const Int &rhs) const;
    bool operator<=(const Int &rhs) const;
    bool operator==(const Value &rhs) const;

    const mpz_t &GetValue() const;

    static Value *ParseInt(const std::string &str);
  private:
    mpz_t myValue;
}; // }}}

}
