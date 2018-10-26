#pragma once

#include <libpi/value.hpp>

namespace libpi
{

// DOCUMENTATION: String class {{{
/*!
 * String implements string operation.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class String : public Value // {{{
{ public:
    enum STRINGOP { OP_CONCAT };
    // Constructors
    String(const std::string &val, gc::GCRegistrant *registrant);
    String(const String &lhs, const String &rhs, STRINGOP op, gc::GCRegistrant *registrant);
    String(gc::GCRegistrant *registrant);
    virtual ~String();

    std::string GetType() const { return "str"; }
    void ToStream(std::ostream &dest) const;
    String *operator+(const String &rhs) const;
    virtual Bool *operator==(const Value &rhs) const;
    virtual Bool *operator<=(const Value &rhs) const;
    virtual Bool *operator<(const Value &rhs) const;
    virtual Bool *operator>=(const Value &rhs) const;
    virtual Bool *operator>(const Value &rhs) const;

    const std::string &GetValue() const { return myValue; }

    static Value *ParseString(std::istream &in, gc::GCRegistrant *registrant);

  private:
    std::string myValue;
}; // }}}

}
