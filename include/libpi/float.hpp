#pragma once

#include <libpi/value.hpp>
#include <gmp.h>

namespace libpi
{

// DOCUMENTATION: Float class {{{
/*!
 * Float implements floating point operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Float : public Value // {{{
{ public:
    enum FLOATOP { OP_ADD, OP_SUB, OP_MULT, OP_DIV };
    // Copy constructor and assignment
    Float(const Float &val, gc::GCRegistrant *registrant);
    Float(const Float &lhs, const Float &rhs, FLOATOP op, gc::GCRegistrant *registrant);
    Float &operator=(const Float &rhs);

    // Constructors
    Float(const std::string &val, gc::GCRegistrant *registrant);
    Float(mpf_t &val, gc::GCRegistrant *registrant, bool clear_arg=false);
    Float(double val, gc::GCRegistrant *registrant);
    virtual ~Float();

    std::string GetType() const { return "flt"; }
    void ToStream(std::ostream &dest) const;
    //! Float addition
    //Float *operator+(const Float &rhs) const;
    //! Float subtraction
    //Float * operator-(const Float &rhs) const;
    //! Float multiplication
    //Float * operator*(const Float &rhs) const;
    //! Float division
    //Float * operator/(const Float &rhs) const;
    //! Float comparison
    Bool *operator==(const Value &rhs) const;
    Bool *operator<=(const Value &rhs) const;
    Bool *operator<(const Value &rhs) const;
    Bool *operator>=(const Value &rhs) const;
    Bool *operator>(const Value &rhs) const;

    const mpf_t &GetValue() const { return myValue; }

    static Value *ParseFloat(std::istream &in, gc::GCRegistrant *registrant);
  private:
    mpf_t myValue;
}; // }}}

}
