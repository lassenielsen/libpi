#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace libpi
{
// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 */
// }}}
class Value // {{{
{ public:
    typedef Value *(*value_creator)(const std::string &);
  public:
    Value();
    Value(const std::string &str);
    virtual ~Value();
    virtual std::string GetType() const;
    virtual std::string ToString() const;
    virtual std::string Serialize() const;
    virtual bool operator==(const Value &rhs) const;

    static Value *Parse(const std::string &str);
    static int RegisterParser(const std::string &type, value_creator p);

  private:
    static std::map<std::string,value_creator> ourParsers;
}; // }}}

//// DOCUMENTATION: ChannelsValue class {{{
///*!
// * ChannelsValue wraps a channnel vector.
// * This allows a straight forward and uniform syntax for performing all
// * operations.
// */
//// }}}
//class ChannelsValue : public Value // {{{
//{ public:
//    // Copy constructor and assignment
//    ChannelsValue(const ChannelsValue &val);
//    ChannelsValue &operator=(const ChannelsValue &rhs);
//    ChannelsValue *Copy() const;
//
//    // Constructors
//    ChannelsValue(Message &msg);
//    ChannelsValue(const std::vector<Channel*> &chs);
//    ChannelsValue();
//    virtual ~ChannelsValue();
//
//    std::string ToString() const;
//    bool operator==(const Value &rhs) const;
//
//    const std::vector<Channel*> &GetValues() const;
//    std::vector<Channel*> &GetValues();
//  private:
//    std::vector<Channel*> myChannels;
//}; // }}}
//
//// DOCUMENTATION: TupleValue class {{{
///*!
// * TupleValue is used to represent tuples, but is implemented as a
// * vector. Thus explicit type-casting must be used when retrieving
// * value.
// */
//// }}}
//class TupleValue : public Value // {{{
//{ public:
//    // Copy constructor and assignment
//    TupleValue(const TupleValue &val);
//    TupleValue &operator=(const TupleValue &rhs);
//    TupleValue *Copy() const;
//
//    // Constructors
//    // FIXME: Implemenr dynamic value creation method
//    //TupleValue(Message &msg);
//    TupleValue();
//    virtual ~TupleValue();
//
//    std::string ToString() const;
//    const Value &GetValue(const IntValue &index) const;
//    const Value &GetValue(int index) const;
//    bool operator==(const Value &rhs) const;
//
//    const std::vector<Value*> &GetValues() const;
//    void AddValue(const Value &val);
//
//
//  private:
//    std::vector<Value*> myValues;
//}; // }}}

}
