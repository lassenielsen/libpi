#pragma once

#include <libpi/value.hpp>
#include <gmp.h>

namespace libpi
{

// DOCUMENTATION: Quotient class {{{
/*!
 * Quotient implements exact quotient objects and operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Quotient : public Value // {{{
{ public:
    enum QUOTOP { OP_ADD, OP_SUB, OP_MULT, OP_DIV };
    // Copy constructor and assignment
    Quotient(const Quotient &val, gc::GCRegistrant *registrant);
    Quotient(const Quotient &lhs, const Quotient &rhs, QUOTOP op, gc::GCRegistrant *registrant);
    Quotient &operator=(const Quotient &rhs);

    // Constructors
    Quotient(gc::GCRegistrant *registrant);
    Quotient(const std::string &val, gc::GCRegistrant *registrant);
    Quotient(mpq_t &val, gc::GCRegistrant *registrant, bool clear_arg=false);
    Quotient(double val, gc::GCRegistrant *registrant);
    virtual ~Quotient();

    std::string GetType() const { return "qrt"; }
    void ToStream(std::ostream &dest) const;
    //! Quotient addition
    //Quotient *operator+(const Quotient &rhs) const;
    //! Quotient subtraction
    //Quotient *operator-(const Quotient &rhs) const;
    //! Quotient multiplication
    //Quotient *operator*(const Quotient &rhs) const;
    //! Quotient division
    //Quotient *operator/(const Quotient &rhs) const;
    //! Quotienteger comparison
    Bool *operator==(const Value &rhs) const;
    Bool *operator<=(const Value &rhs) const;
    Bool *operator<(const Value &rhs) const;
    Bool *operator>=(const Value &rhs) const;
    Bool *operator>(const Value &rhs) const;

    const mpq_t &GetValue() const {return myValue;}
    mpq_t &GetValue() {return myValue;}

    static Value * ParseQuotient(std::istream &in, gc::GCRegistrant *registrant);

  private:
    mpq_t myValue;
}; // }}}

}
