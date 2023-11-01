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
    void ToStream(std::ostream &dest) const;
    //! Integer addition
    Int *operator+(const Int &rhs) const;
    //! Integer subtraction
    Int *operator-(const Int &rhs) const;
    //! Integer multiplication
    Int *operator*(const Int &rhs) const;
    //! Integer division
    Int *operator/(const Int &rhs) const;
    //! Integer comparison
    bool operator==(const Value &rhs) const;
    bool operator<=(const Value &rhs) const;
    bool operator<(const Value &rhs) const;
    bool operator>=(const Value &rhs) const;
    bool operator>(const Value &rhs) const;

    const mpz_t &GetValue() const;

    static Value *ParseInt(std::istream &in);
  private:
    mpz_t myValue;
}; // }}}

}
