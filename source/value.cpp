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
queue<Value*> Value::ourGarbage;
pthread_mutex_t Value::ourGarbageLock;
//pthread_mutex_t Value::ourGarbageWaitLock;
int initValueLocks=Value::InitLocks();

// Value Implementation
Value::Value() // {{{
: myRefCount(1)
{
} // }}}
Value::Value(const string &str) // {{{
{ if (str!="()")
    throw string("Value constructor expected string '()' but received: ") + str;
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
bool Value::operator==(const Value &rhs) const // {{{
{ return typeid(rhs)==typeid(*this); 
} // }}}
bool Value::operator<=(const Value &rhs) const // {{{
{ return typeid(rhs)==typeid(*this); 
} // }}}
bool Value::operator<(const Value &rhs) const // {{{
{ return false; 
} // }}}
bool Value::operator>=(const Value &rhs) const // {{{
{ return typeid(rhs)==typeid(*this); 
} // }}}
bool Value::operator>(const Value &rhs) const // {{{
{ return false; 
} // }}}
Value* Value::Parse(const string &str) // {{{
{ stringstream ss;
  ss << str;
  return Parse(ss);
} // }}}
Value* Value::Parse(istream &in) // {{{
{ char delimiter=':';
  string type;
  std::getline(in,type,delimiter);
  map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser==ourParsers.end())
    throw std::string("Unable to parse value of type: ")+type;
  return (parser->second)(in);
} // }}}
int Value::RegisterParser(const string &type, value_creator p) // {{{
{ map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser!=ourParsers.end())
    throw std::string("Value::RegisterParser: Type (") + type + ")already registered.";
  ourParsers[type]=p;
  return 0;
} // }}}

}
