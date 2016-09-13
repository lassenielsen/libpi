#pragma once
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: Bool class {{{
/*!
 * Bool implements boolean operations.
 * This allows a straight forward and uniform syntax for performing all
 * operations.
 */
// }}}
class Bool : public Value // {{{
{ public:
    // Copy constructor and assignment
    Bool(const Bool &val);

    // Constructors
    Bool(const std::string &str);
    Bool(bool val=false);
    virtual ~Bool();

    std::string GetType() const { return "boo"; }
    void ToString(std::ostream &dest) const;
    std::shared_ptr<Bool> operator&&(const Bool &rhs) const;
    std::shared_ptr<Bool> operator||(const Bool &rhs) const;
    std::shared_ptr<Bool> operator!() const;
    bool operator==(const Value &rhs) const;

    bool GetValue() const;

    static Value *ParseBool(std::istream &in);

  private:
    bool myValue;
}; // }}}

}
