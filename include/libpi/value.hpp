#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>

namespace libpi
{
// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
class Value // {{{
{ public:
    typedef Value *(*value_creator)(std::istream &);
  public:
    Value();
    Value(const std::string &str);
    virtual ~Value();
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    std::string ToString() const;
    void Serialize(std::ostream &dest) const;
    std::string Serialize() const;
    virtual bool operator==(const Value &rhs) const;

    static Value *Parse(const std::string &str);
    static Value *Parse(std::istream &in);
    static int RegisterParser(const std::string &type, value_creator p);

  private:
    static std::map<std::string,value_creator> ourParsers;
}; // }}}

}
