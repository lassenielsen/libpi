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
    // Copy constructor and assignment
    Float(const Float &val);
    Float &operator=(const Float &rhs);

    // Constructors
    Float(const std::string &val);
    Float(mpf_t &val, bool clear_arg=false);
    Float(double val=0);
    virtual ~Float();

    std::string GetType() const { return "flt"; }
    void ToStream(std::ostream &dest) const;
    //! Floateger addition
    Float *operator+(const Float &rhs) const;
    //! Floateger subtraction
    Float *operator-(const Float &rhs) const;
    //! Floateger multiplication
    Float *operator*(const Float &rhs) const;
    //! Floateger division
    Float *operator/(const Float &rhs) const;
    //! Float comparison
    bool operator==(const Value &rhs) const;
    bool operator<=(const Value &rhs) const;
    bool operator<(const Value &rhs) const;
    bool operator>=(const Value &rhs) const;
    bool operator>(const Value &rhs) const;

    const mpf_t &GetValue() const { return myValue; }

    static Value *ParseFloat(std::istream &in);
  private:
    mpf_t myValue;
}; // }}}

}
