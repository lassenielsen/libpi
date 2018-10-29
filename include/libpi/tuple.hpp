#pragma once
#include <libpi/value.hpp>
#include <libpi/int.hpp>
#include <vector>

namespace libpi
{

//// DOCUMENTATION: Tuple class {{{
///*!
// * Tuple is used to represent tuples, but is implemented as a
// * vector. Thus explicit type-casting must be used when retrieving
// * value.
// */
//// }}}
class Tuple : public Value // {{{
{ public:
    // Constructors
    Tuple(gc::GCRegistrant *registrant);
    Tuple(std::vector<Value*> &vals, gc::GCRegistrant *registrant);
    virtual ~Tuple();

    std::string GetType() const { return "tpl"; }
    void ToStream(std::ostream &dest) const;
    const Value *GetValue(const Int &index) const;
    Value *GetValue(const Int &index);
    const Value *GetValue(int index) const;
    Value *GetValue(int index);
    Bool *operator==(const Value &rhs) const;
    Bool *operator<=(const Value &rhs) const;
    Bool *operator<(const Value &rhs) const;
    Bool *operator>=(const Value &rhs) const;
    Bool *operator>(const Value &rhs) const;

    const std::vector<Value*> &GetValues() const {return myValues; }
    std::vector<Value*> &GetValues() { return myValues; }
    void AddValue(Value *val);

    virtual void Mark(std::unordered_set<Value*> &marks);

    static Value *ParseTuple(std::istream &in, gc::GCRegistrant *registrant);

  private:
    std::vector<Value*> myValues;
}; // }}}

}
