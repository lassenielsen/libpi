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
    Tuple();
    Tuple(std::vector<Value*> &vals);
    virtual ~Tuple();

    std::string GetType() const { return "tpl"; }
    void ToStream(std::ostream &dest) const;
    const Value *GetValue(const Int &index) const;
    Value *GetValue(const Int &index);
    const Value *GetValue(int index) const;
    Value *GetValue(int index);
    bool operator==(const Value &rhs) const;
    bool operator<=(const Value &rhs) const;
    bool operator<(const Value &rhs) const;
    bool operator>=(const Value &rhs) const;
    bool operator>(const Value &rhs) const;

    const std::vector<Value*> &GetValues() const {return myValues; }
    std::vector<Value*> &GetValues() { return myValues; }
    void AddValue(Value *val);

    static Value *ParseTuple(std::istream &in);

  private:
    std::vector<Value*> myValues;
}; // }}}

}
