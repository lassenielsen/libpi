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
    // Copy constructor and assignment
    String(const String &val);
    String &operator=(const String &rhs);

    // Constructors
    String(const std::string &val);
    String();
    virtual ~String();

    std::string GetType() const { return "str"; }
    std::string ToString() const;
    String operator+(const String &rhs) const;
    bool operator==(const Value &rhs) const;

    const std::string &GetValue() const;

    static Value *ParseString(const std::string &str);

  private:
    std::string myValue;
}; // }}}

}
