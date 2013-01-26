#ifndef PIVALUE_HPP
#define PIVALUE_HPP

#include <libpi/message.hpp>

namespace libpi
{
// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all values.
 */
// }}}
class Value // {{{
{ virtual ~Valut();
  virtual void ToMessage(Message &dest)=0;
}; // }}}

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

// DOCUMENTATION: StringValue class {{{
/*!
 * StringValue implements string operation.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class StringValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    StringValue(const StringValue &val);
    StringValue &operator=(const StringValue &rhs);

    // Constructors
    StringValue(Message &msg);
    StringValue(const std::string &val);
    virtual ~StringValue();

    void ToMessage(Message &dest);
    StringValue operator+(const StringValue &rhs);
    bool operator==(const StringValue &rhs);

    const std::string &GetValue() const;
  private:
    std::string myValue;
}; // }}}

// DOCUMENTATION: BoolValue class {{{
/*!
 * BoolValue implements boolean operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class BoolValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    BoolValue(const BoolValue &val);
    BoolValue &operator=(const BoolValue &rhs);

    // Constructors
    BoolValue(Message &msg);
    BoolValue(bool val);
    virtual ~BoolValue();

    void ToMessage(Message &dest);
    BoolValue operator+(const BoolValue &rhs);
    bool operator==(const BoolValue &rhs);

    bool GetValue() const;
  private:
    bool myValue;
}; // }}}

// DOCUMENTATION: TupleValue class {{{
/*!
 * TupleValue is used to represent tuples, but is implemented as a
 * vector. Thus explicit type-casting must be used when retrieving
 * value.
 */
// }}}
class TupleValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    TupleValue(const TupleValue &val);
    TupleValue &operator=(const TupleValue &rhs);

    // Constructors
    TupleValue(Message &msg);
    TupleValue();
    virtual ~TupleValue();

    void AddValue(Value *val);

    void ToMessage(Message &dest);
    Value &GetValue(const IntValue &rhs);
    Value &GetValue(int index);
    bool operator==(const TupleValue &rhs);

  private:
    vector<Value*> myValues;
}; // }}}

}

#endif
