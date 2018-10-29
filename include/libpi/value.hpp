#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <unordered_set>
#include <libpi/gc/registrant.hpp>

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
    typedef Value *(*value_creator)(std::istream &, gc::GCRegistrant *registrant);
  public:
    Value(gc::GCRegistrant *registrant);
    Value(const std::string &str, gc::GCRegistrant *registrant);
    virtual ~Value();
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    std::string ToString() const;
    void Serialize(std::ostream &dest) const;
    std::string Serialize() const;
    virtual Bool *operator==(const Value &rhs) const;
    virtual Bool *operator<=(const Value &rhs) const;
    virtual Bool *operator<(const Value &rhs) const;
    virtual Bool *operator>=(const Value &rhs) const;
    virtual Bool *operator>(const Value &rhs) const;

    static Value *Parse(const std::string &str, gc::GCRegistrant *registrant);
    static Value *Parse(std::istream &in, gc::GCRegistrant *registrant);
    static int RegisterParser(const std::string &type, value_creator p);

    // Mark function for Garbage Collection
    virtual void Mark(std::unordered_set<Value*> &marks);

  private:
    static std::map<std::string,value_creator> ourParsers;
}; // }}}

}
