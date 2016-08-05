#include <libpi/value.hpp>
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
Value::Value() // {{{
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
std::string Value::ToString() const // {{{
{ return "()";
} // }}}
std::string Value::Serialize() const // {{{
{ return GetType() + ":" + ToString();
} // }}}
Value *Value::Copy() const // {{{
{ return new Value();
} // }}}
bool Value::operator==(const Value &rhs) const // {{{
{ return typeid(rhs)==typeid(*this); 
} // }}}
Value *Value::Parse(const string &str) // {{{
{ size_t pos=str.find(':');
  string type=str.substr(0,pos);
  map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser==ourParsers.end())
    throw std::string("Unable to parse value of type: ")+type;
  return (parser->second)(str.substr(pos+1));
} // }}}
int Value::RegisterParser(const string &type, value_creator p) // {{{
{ map<string,value_creator>::const_iterator parser=ourParsers.find(type);
  if (parser!=ourParsers.end())
    throw std::string("Value::RegisterParser: Type (") + type + ")already registered.";
  ourParsers[type]=p;
  return 0;
} // }}}

//ChannelsValue::ChannelsValue(const ChannelsValue &val) // {{{
//{ for (vector<Channel*>::const_iterator it=val.GetValues().begin();
//       it!=val.GetValues().end();
//       ++it)
//    myChannels.push_back((*it)->Copy());
//} // }}}
//ChannelsValue &ChannelsValue::operator=(const ChannelsValue &rhs) // {{{
//{ myChannels.clear();
//  for (vector<Channel*>::const_iterator it=rhs.GetValues().begin();
//       it!=rhs.GetValues().end();
//       ++it)
//    myChannels.push_back((*it)->Copy());
//} // }}}
//ChannelsValue *ChannelsValue::Copy() const // {{{
//{
//  return new ChannelsValue(*this);
//} // }}}
//
//vector<string> split(const std::string &s, char delim) // {{{
//{
//  vector<string> result;
//  stringstream ss(s);
//  string item;
//  while (getline(ss, item, delim)) {
//      result.push_back(item);
//  }
//  return result;
//} // }}}
//ChannelsValue::ChannelsValue(Message &val) // {{{
//{ vector<string> addresses=split((string)val.GetData(),';');
//  for (vector<string>::const_iterator it=addresses.begin();
//       it!=addresses.end();
//       ++it)
//    myChannels.push_back(Channel::Create(*it));
//} // }}}
//ChannelsValue::ChannelsValue(const vector<Channel*> &chs) // {{{
//{ for (vector<Channel*>::const_iterator it=chs.begin();
//       it!=chs.end();
//       ++it)
//    myChannels.push_back((*it)->Copy());
//} // }}}
//ChannelsValue::ChannelsValue() // {{{
//{
//} // }}}
//ChannelsValue::~ChannelsValue() // {{{
//{ while (myChannels.size()>0)
//  { delete myChannels.back();
//    myChannels.pop_back();
//  }
//} // }}}
//
//string ChannelsValue::ToString() const // {{{
//{ stringstream result;
//  for (vector<Channel*>::const_iterator it=myChannels.begin();
//       it!=myChannels.end();
//       ++it)
//  { if (it!=myChannels.begin())
//      result << ";";
//    result << (*it)->GetAddress();
//  }
//  return result.str();
//} // }}}
//bool ChannelsValue::operator==(const Value &rhs) const // {{{
//{ const ChannelsValue *rhsptr=dynamic_cast<const ChannelsValue*>(&rhs);
//  if (rhsptr==NULL)
//    return false;
//  if (myChannels.size()!=rhsptr->GetValues().size())
//    return false;
//  for (vector<Channel*>::const_iterator lhsCh=myChannels.begin(), rhsCh=rhsptr->GetValues().begin(); lhsCh!=myChannels.end() && rhsCh!=rhsptr->GetValues().end(); ++lhsCh, ++rhsCh)
//    if ((*lhsCh)->GetAddress()!=(*rhsCh)->GetAddress())
//      return false;
//  return true;
//} // }}}
//const vector<Channel*> &ChannelsValue::GetValues() const // {{{
//{ return myChannels;
//} // }}}
//vector<Channel*> &ChannelsValue::GetValues() // {{{
//{ return myChannels;
//} // }}}
//
//// TupleValue Implementation
//TupleValue::TupleValue(const TupleValue &val) // {{{
//{ for (vector<Value*>::const_iterator it=val.GetValues().begin();
//       it!=val.GetValues().end(); ++it)
//    myValues.push_back((*it)->Copy());
//} // }}}
//TupleValue &TupleValue::operator=(const TupleValue &rhs) // {{{
//{ DeleteVector(myValues);
//  for (vector<Value*>::const_iterator it=rhs.GetValues().begin();
//       it!=rhs.GetValues().end(); ++it)
//    myValues.push_back((*it)->Copy());
//} // }}}
//TupleValue *TupleValue::Copy() const // {{{
//{ return new TupleValue(*this);
//} // }}}
//
////TupleValue::TupleValue(Message &msg) // {{{
////{ string msg_val=msg.GetData();
////  vector<string> vals=stringsplit(msg_val,"\\1");
////  for (vector<string>::const_iterator it=vals.begin(); it!=vals.end(); ++it)
////    myValues.push_back(Value::Create(stringreplace(*it,"\\2","\\")); // Add value to myValues
////} // }}}
//TupleValue::TupleValue() // {{{
//{
//} // }}}
//TupleValue::~TupleValue() // {{{
//{ DeleteVector(myValues);
//} // }}}
//
//string TupleValue::ToString() const // {{{
//{ stringstream result;
//  for (vector<Value*>::const_iterator it=myValues.begin();
//       it!=myValues.end(); ++it)
//  { if (it!=myValues.begin())
//      result << "\\1";
//    result << stringreplace((*it)->ToString(),"\\","\\2");
//  }
//  return result.str();
//} // }}}
//const Value &TupleValue::GetValue(const IntValue &index) const // {{{
//{ return GetValue(mpz_get_ui(index.GetValue()));
//} // }}}
//const Value &TupleValue::GetValue(int index) const // {{{
//{ return *(myValues[index]);
//} // }}}
//bool TupleValue::operator==(const Value &rhs) const // {{{
//{ const TupleValue *rhsptr=dynamic_cast<const TupleValue*>(&rhs);
//  if (rhsptr==NULL)
//    return false;
//  if (rhsptr->GetValues().size()!=myValues.size())
//    return false;
//  for (int i=0; i<myValues.size(); ++i)
//    if (!(GetValue(i)==rhsptr->GetValue(i)))
//      return false;
//  return true;
//} // }}}
//const vector<Value*> &TupleValue::GetValues() const // {{{
//{ return myValues;
//} // }}}
//void TupleValue::AddValue(const Value &val) // {{{
//{ myValues.push_back(val.Copy());
//} // }}}

}
