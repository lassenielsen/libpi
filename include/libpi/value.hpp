#ifndef PIVALUE_HPP
#define PIVALUE_HPP

#include <vector>
#include <gmp.h>
#include <string>

namespace libpi
{
  class Message;
  class Channel;
// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all values.
 */
// }}}
class Value // {{{
{ public:
    virtual ~Value();
    void ToMessage(Message &dest) const;
    virtual std::string ToString() const=0;
    virtual Value *Copy() const=0;
    virtual bool operator==(const Value &rhs) const=0;
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
    IntValue *Copy() const;

    // Constructors
    IntValue(Message &msg);
    IntValue(const std::string &val);
    IntValue(mpz_t &val, bool clear_arg=false);
    IntValue(long val=0);
    virtual ~IntValue();

    std::string ToString() const;
    IntValue operator+(const IntValue &rhs) const;
    IntValue operator-(const IntValue &rhs) const;
    IntValue operator*(const IntValue &rhs) const;
    IntValue operator/(const IntValue &rhs) const;
    bool operator<=(const IntValue &rhs) const;
    bool operator==(const Value &rhs) const;

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
    StringValue *Copy() const;

    // Constructors
    StringValue(Message &msg);
    StringValue(const std::string &val);
    StringValue();
    virtual ~StringValue();

    std::string ToString() const;
    StringValue operator+(const StringValue &rhs) const;
    bool operator==(const Value &rhs) const;

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
    BoolValue *Copy() const;

    // Constructors
    BoolValue(Message &msg);
    BoolValue(bool val=false);
    virtual ~BoolValue();

    std::string ToString() const;
    BoolValue operator&&(const BoolValue &rhs) const;
    BoolValue operator||(const BoolValue &rhs) const;
    BoolValue operator!() const;
    bool operator==(const Value &rhs) const;

    bool GetValue() const;
  private:
    bool myValue;
}; // }}}

// DOCUMENTATION: ChannelsValue class {{{
/*!
 * ChannelsValue wraps a channnel vector.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class ChannelsValue : public Value // {{{
{ public:
    // Copy constructor and assignment
    ChannelsValue(const ChannelsValue &val);
    ChannelsValue &operator=(const ChannelsValue &rhs);
    ChannelsValue *Copy() const;

    // Constructors
    ChannelsValue(Message &msg);
    ChannelsValue(const std::vector<Channel*> &chs);
    ChannelsValue();
    virtual ~ChannelsValue();

    std::string ToString() const;
    bool operator==(const Value &rhs) const;

    const std::vector<Channel*> &GetValues() const;
    std::vector<Channel*> &GetValues();
  private:
    std::vector<Channel*> myChannels;
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
    TupleValue *Copy() const;

    // Constructors
    // FIXME: Implemenr dynamic value creation method
    //TupleValue(Message &msg);
    TupleValue();
    virtual ~TupleValue();

    std::string ToString() const;
    const Value &GetValue(const IntValue &index) const;
    const Value &GetValue(int index) const;
    bool operator==(const Value &rhs) const;

    const std::vector<Value*> &GetValues() const;
    void AddValue(const Value &val);


  private:
    std::vector<Value*> myValues;
}; // }}}

}

#endif
