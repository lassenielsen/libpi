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
    enum INTOP { OP_ADD, OP_SUB, OP_MULT, OP_DIV };
    // Copy constructor and assignment
    Int(const Int &val, gc::GCRegistrant *registrant);
    Int(const Int &lhs, const Int &rhs, INTOP op, gc::GCRegistrant *registrant);
    Int &operator=(const Int &rhs);

    // Constructors
    Int(const std::string &val, gc::GCRegistrant *registrant);
    Int(mpz_t &val, gc::GCRegistrant *registrant, bool clear_arg=false);
    Int(long val, gc::GCRegistrant *registrant);
    virtual ~Int();

    std::string GetType() const { return "int"; }
    void ToStream(std::ostream &dest) const;
    //! Integer addition
    //Int *operator+(const Int &rhs) const;
    //! Integer subtraction
    //Int *operator-(const Int &rhs) const;
    //! Integer multiplication
    //Int *operator*(const Int &rhs) const;
    //! Integer division
    //Int *operator/(const Int &rhs) const;
    //! Integer comparison
    Bool *operator==(const Value &rhs) const;
    Bool *operator<=(const Value &rhs) const;
    Bool *operator<(const Value &rhs) const;
    Bool *operator>=(const Value &rhs) const;
    Bool *operator>(const Value &rhs) const;

    const mpz_t &GetValue() const;

    static Value *ParseInt(std::istream &in, gc::GCRegistrant *registrant);
  private:
    mpz_t myValue;
}; // }}}

}
