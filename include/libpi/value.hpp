#ifndef PIVALUE_HPP
#define PIVALUE_HPP

#include <libpi/message.hpp>

namespace libpi
{

// DOCUMENTATION: Value class {{{
/*!
 * IntValue implements integer operation.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class IntValue // {{{
{ public:
    // Copy constructor and assignment
    IntValue(const IntValue &val);
    IntValue &operator=(const IntValue &rhs);

    // Constructors
    IntValue(Message &msg);
    IntValue(const std::string &val);
    IntValue(mpz_t &val, bool clear_arg=false);
    IntValue(long val);
    virtual ~IntValue();

    void ToMessage(Message &dest);
    IntValue operator+(const IntValue &rhs);
    IntValue operator-(const IntValue &rhs);
    IntValue operator*(const IntValue &rhs);
    IntValue operator/(const IntValue &rhs);
    bool operator<=(const IntValue &rhs);
    bool operator==(const IntValue &rhs);

    const mpz_t &GetValue() const;
  private:
    mpz_t myValue;
}; // }}}

}

#endif
