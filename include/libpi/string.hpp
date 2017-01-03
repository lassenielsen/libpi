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
    // Constructors
    String(const std::string &val);
    String();
    virtual ~String();

    std::string GetType() const { return "str"; }
    void ToStream(std::ostream &dest) const;
    std::shared_ptr<String> operator+(const String &rhs) const;
    virtual std::shared_ptr<Bool> operator==(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>(const Value &rhs) const;

    const std::string &GetValue() const { return myValue; }

    static std::shared_ptr<Value> ParseString(std::istream &in);

  private:
    std::string myValue;
}; // }}}

}
