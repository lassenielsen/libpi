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
    void ToStream(std::ostream &dest) const;
    //! Quotient addition
    std::shared_ptr<Quotient> operator+(const Quotient &rhs) const;
    //! Quotient subtraction
    std::shared_ptr<Quotient> operator-(const Quotient &rhs) const;
    //! Quotient multiplication
    std::shared_ptr<Quotient> operator*(const Quotient &rhs) const;
    //! Quotient division
    std::shared_ptr<Quotient> operator/(const Quotient &rhs) const;
    //! Quotienteger comparison
    std::shared_ptr<Bool> operator==(const Value &rhs) const;
    std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    std::shared_ptr<Bool> operator<(const Value &rhs) const;
    std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    std::shared_ptr<Bool> operator>(const Value &rhs) const;

    const mpq_t &GetValue() const {return myValue;}
    mpq_t &GetValue() {return myValue;}

    static Value *ParseQuotient(std::istream &in);

  private:
    mpq_t myValue;
}; // }}}

}
