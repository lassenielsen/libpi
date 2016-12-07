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
    Tuple(std::vector<std::shared_ptr<Value> > &vals);
    virtual ~Tuple();

    std::string GetType() const { return "tpl"; }
    void ToStream(std::ostream &dest) const;
    const std::shared_ptr<Value> &GetValue(const Int &index) const;
    std::shared_ptr<Value> &GetValue(const Int &index);
    const std::shared_ptr<Value> &GetValue(int index) const;
    std::shared_ptr<Value> &GetValue(int index);
    bool operator==(const Value &rhs) const;

    const std::vector<std::shared_ptr<Value> > &GetValues() const;
    std::vector<std::shared_ptr<Value> > &GetValues();
    void AddValue(std::shared_ptr<Value> val);

    static Value *ParseTuple(std::istream &in);

  private:
    std::vector<std::shared_ptr<Value> > myValues;
}; // }}}

}
