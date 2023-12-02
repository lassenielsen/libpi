#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <iostream>
#include <atomic>

namespace libpi
{

// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 * Defines interface for basic operations, and also holds the reference counting functionality.
 */
// }}}
class Value // {{{
{ public:
    typedef Value* (*value_creator)(std::istream &);
  public:
    Value();
    Value(const std::string &str);
    virtual ~Value();

    int RefCount() const { return myRefCount; }
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    std::string ToString() const;
    void Serialize(std::ostream &dest) const;
    std::string Serialize() const;
    virtual bool operator==(const Value &rhs) const;
    virtual bool operator<=(const Value &rhs) const;
    virtual bool operator<(const Value &rhs) const;
    virtual bool operator>=(const Value &rhs) const;
    virtual bool operator>(const Value &rhs) const;

    static Value* Parse(const std::string &str);
    static Value* Parse(std::istream &in);
    static int RegisterParser(const std::string &type, value_creator p);

    inline void AddRef() { ++myRefCount; }
    friend void RemoveRef(Value *ref);
  private:
    static std::map<std::string,value_creator> ourParsers;
    std::atomic<int> myRefCount;
}; // }}}


inline void RemoveRef(Value *ref) { --(ref->myRefCount); if (ref->myRefCount<=0) { /*std::cout << "Deleting value at: " << (long int)this << std::endl;*/ delete ref;} }
inline void AssignValue(Value **var, Value *val) // {{{
{ if (val==*var)
    return;
  val->AddRef();
  if (*var)
    RemoveRef(*var);
  *var=val;
} // }}}
inline void AssignNewValue(Value **var, Value *val) // {{{
{ if (*var)
    RemoveRef(*var);
  *var=val;
} // }}}
}
