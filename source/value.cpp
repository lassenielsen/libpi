#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <string.h>
#include "common.cpp"
#include <stdlib.h>
#include <libpi/message.hpp>
#include <libpi/channel.hpp>
#include <typeinfo>

using namespace std;
namespace libpi
{

map<string,Value::value_creator> Value::ourParsers;
// Value Implementation
Value::Value(gc::GCRegistrant *registrant) // {{{
{ if (registrant!=NULL)
    registrant->GCRegister((void*)this);
} // }}}
Value::Value(const string &str, gc::GCRegistrant *registrant) // {{{
{ if (str!="()")
    throw string("Value constructor expected string '()' but received: ") + str;
  if (registrant!=NULL)
    registrant->GCRegister((void*)this);
} // }}}
Value::~Value() // {{{
{
} // }}}
std::string Value::GetType() const // {{{
{ return "unt";
} // }}}
void Value::ToStream(ostream &dest) const // {{{
{ return;
} // }}}
std::string Value::ToString() const // {{{
{ stringstream ss;
  ToStream(ss);
  return ss.str();
} // }}}
void Value::Serialize(ostream &dest) const // {{{
{ dest << GetType() << ":";
  ToStream(dest);
} // }}}
std::string Value::Serialize() const // {{{
{ stringstream ss;
  Serialize(ss);
  return ss.str();
} // }}}
Bool *Value::operator==(const Value &rhs) const // {{{
{ return Bool::GetInstance(typeid(rhs)==typeid(*this)); 
} // }}}
Bool *Value::operator<=(const Value &rhs) const // {{{
{ return Bool::GetInstance(typeid(rhs)==typeid(*this)); 
} // }}}
Bool *Value::operator<(const Value &rhs) const // {{{
{ return Bool::GetInstance(false); 
} // }}}
Bool *Value::operator>=(const Value &rhs) const // {{{
{ return Bool::GetInstance(typeid(rhs)==typeid(*this)); 
} // }}}
Bool *Value::operator>(const Value &rhs) const // {{{
{ return Bool::GetInstance(false); 
} // }}}
    void Value::Mark(unordered_set<Value*> &marks) // {{{
    { if (marks.count(this)>0)
        return;
      marks.insert(this);
    } // }}}
Value *Value::Parse(const string &str, gc::GCRegistrant *registrant) // {{{
{ stringstream ss;
  ss << str;
  return Parse(ss,registrant);
} // }}}
Value *Value::Parse(istream &in, gc::GCRegistrant *registrant) // {{{
{ char delimiter=':';
  string type;
  std::getline(in,type,delimiter);
  map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser==ourParsers.end())
    throw std::string("Unable to parse value of type: ")+type;
  return (parser->second)(in,registrant);
} // }}}
int Value::RegisterParser(const string &type, value_creator p) // {{{
{ map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser!=ourParsers.end())
    throw std::string("Value::RegisterParser: Type (") + type + ")already registered.";
  ourParsers[type]=p;
  return 0;
} // }}}

}
