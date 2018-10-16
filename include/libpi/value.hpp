#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <unordered_set>

namespace libpi
{
class Bool;

// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
class Value // {{{
{ public:
    typedef std::shared_ptr<Value> (*value_creator)(std::istream &);
  public:
    Value();
    Value(const std::string &str);
    virtual ~Value();
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    std::string ToString() const;
    void Serialize(std::ostream &dest) const;
    std::string Serialize() const;
    virtual std::shared_ptr<Bool> operator==(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>(const Value &rhs) const;

    static std::shared_ptr<Value> Parse(const std::string &str);
    static std::shared_ptr<Value> Parse(std::istream &in);
    static int RegisterParser(const std::string &type, value_creator p);

    // Mark function for Garbage Collection
    virtual void Mark(std::unordered_set<void *> &marks);

  private:
    static std::map<std::string,value_creator> ourParsers;
}; // }}}

}
