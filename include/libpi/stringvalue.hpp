#pragma once

#include <libpi/value.hpp>

namespace libpi
{

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
    StringValue(const std::string &val);
    StringValue();
    virtual ~StringValue();

    std::string GetType() const { return "str"; }
    std::string ToString() const;
    StringValue operator+(const StringValue &rhs) const;
    bool operator==(const Value &rhs) const;

    const std::string &GetValue() const;

    static Value *ParseString(const std::string &str);

  private:
    std::string myValue;
}; // }}}

}
