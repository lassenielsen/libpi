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
    // Copy constructor and assignment
    Quotient(const Quotient &val);
    Quotient &operator=(const Quotient &rhs);

    // Constructors
    Quotient();
    Quotient(const std::string &val);
    Quotient(mpq_t &val, bool clear_arg=false);
    Quotient(double vald);
    virtual ~Quotient();

    std::string GetType() const { return "qrt"; }
    void ToString(std::ostream &dest) const;
    //! Quotienteger addition
    std::shared_ptr<Quotient> operator+(const Quotient &rhs) const;
    //! Quotienteger subtraction
    std::shared_ptr<Quotient> operator-(const Quotient &rhs) const;
    //! Quotienteger multiplication
    std::shared_ptr<Quotient> operator*(const Quotient &rhs) const;
    //! Quotienteger division
    std::shared_ptr<Quotient> operator/(const Quotient &rhs) const;
    //! Quotienteger comparison
    bool operator<=(const Quotient &rhs) const;
    //! Quotienteger comparison
    bool operator==(const Value &rhs) const;

    const mpq_t &GetValue() const {return myValue;}
    mpq_t &GetValue() {return myValue;}

    static Value *ParseQuotient(std::istream &in);

  private:
    mpq_t myValue;
}; // }}}

}