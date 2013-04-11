#include <libpi/value.hpp>
#include <string.h>
#include "common.cpp"
#include <stdlib.h>
#include <libpi/message.hpp>
#include <libpi/channel_mq.hpp>

using namespace std;
namespace libpi
{

// Value (partial) Implementation
Value::~Value() // {{{
{
} // }}}
void Value::ToMessage(Message &dest) const // {{{
{ string str=ToString();
  dest.AddData(str.c_str(),str.size()+1);
} // }}}

// IntValue Implementation
IntValue::IntValue(const IntValue &val) // {{{
{ mpz_init(myValue);
  mpz_set(myValue,val.GetValue());
} // }}}
IntValue &IntValue::operator=(const IntValue &rhs) // {{{
{ mpz_set(myValue,rhs.GetValue());
} // }}}
IntValue *IntValue::Copy() const // {{{
{ return new IntValue(*this);
} // }}}

IntValue::IntValue(Message &msg) // {{{
{ mpz_init(myValue);
  mpz_set_str(myValue,msg.GetData(),10);
} // }}}
IntValue::IntValue(const std::string &val) // {{{
{ mpz_init(myValue);
  mpz_set_str(myValue,val.c_str(),10);
} // }}}
IntValue::IntValue(mpz_t &val, bool clear_arg) // {{{
{ mpz_init(myValue);
  mpz_set(myValue,val);
  if (clear_arg)
    mpz_clear(val);
} // }}}
IntValue::IntValue(long val) // {{{
{ mpz_init(myValue);
  mpz_set_si(myValue,val);
} // }}}

IntValue::~IntValue() // {{{
{ // Free value
  mpz_clear(myValue);
} // }}}

string IntValue::ToString() const // {{{
{ char *str=mpz_get_str(NULL,10,myValue);
  string result=str;
  free(str);
  return result;
} // }}}
IntValue IntValue::operator+(const IntValue &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_add(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator-(const IntValue &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_sub(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator*(const IntValue &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_mul(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator/(const IntValue &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_tdiv_q(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
bool IntValue::operator<=(const IntValue &rhs) const // {{{
{ int cmp = mpz_cmp(myValue,rhs.GetValue());
  return cmp<=0;
} // }}}
bool IntValue::operator==(const Value &rhs) const // {{{
{ const IntValue *rhsptr=dynamic_cast<const IntValue*>(&rhs);
  if (rhsptr=NULL)
    return false;
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return cmp==0;
} // }}}
const mpz_t &IntValue::GetValue() const // {{{
{ return myValue;
} // }}}

// StringValue Implementation
StringValue::StringValue(const StringValue &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}
StringValue &StringValue::operator=(const StringValue &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}
StringValue *StringValue::Copy() const // {{{
{ return new StringValue(*this);
} // }}}

StringValue::StringValue(Message &msg) // {{{
{ myValue=msg.GetData();
} // }}}
StringValue::StringValue(const std::string &val) // {{{
{ myValue=val;
} // }}}
StringValue::StringValue() // {{{
{ myValue="";
} // }}}

StringValue::~StringValue() // {{{
{
} // }}}

string StringValue::ToString() const // {{{
{ return myValue;
} // }}}

StringValue StringValue::operator+(const StringValue &rhs) const // {{{
{ return StringValue(myValue+rhs.GetValue());
} // }}}
bool StringValue::operator==(const Value &rhs) const // {{{
{ const StringValue *rhsptr=dynamic_cast<const StringValue*>(&rhs);
  if (rhsptr=NULL)
    return false;
  return myValue == rhsptr->GetValue();
} // }}}
const std::string &StringValue::GetValue() const // {{{
{ return myValue;
} // }}}

// BoolValue Implementation
BoolValue::BoolValue(const BoolValue &val) // {{{
{ myValue = val.GetValue();
} // }}}
BoolValue &BoolValue::operator=(const BoolValue &rhs) // {{{
{ myValue = rhs.GetValue();
} // }}}
BoolValue *BoolValue::Copy() const // {{{
{ return new BoolValue(*this);
} // }}}

BoolValue::BoolValue(Message &msg) // {{{
{ string msg_val = msg.GetData();
  if (msg_val=="true")
    myValue = true;
  else if (msg_val=="false")
    myValue=false;
  else throw (string)"BoolValue::BoolValue: Bad message value: " +msg_val;
} // }}}
BoolValue::BoolValue(bool val) // {{{
{ myValue=val;
} // }}}
BoolValue::~BoolValue() // {{{
{
} // }}}

string BoolValue::ToString() const // {{{
{ if (myValue)
    return "true";
  else
    return "false";
} // }}}
BoolValue BoolValue::operator&&(const BoolValue &rhs) const // {{{
{ return BoolValue(GetValue() && rhs.GetValue());
} // }}}
BoolValue BoolValue::operator||(const BoolValue &rhs) const // {{{
{ return BoolValue(GetValue() || rhs.GetValue());
} // }}}
BoolValue BoolValue::operator!() const // {{{
{ return BoolValue(!GetValue());
} // }}}
bool BoolValue::operator==(const Value &rhs) const // {{{
{ const BoolValue *rhsptr=dynamic_cast<const BoolValue*>(&rhs);
  if (rhsptr=NULL)
    return false;
  return myValue==rhsptr->GetValue();
} // }}}

bool BoolValue::GetValue() const // {{{
{ return myValue;
} // }}}

MQChannelValue::MQChannelValue(const MQChannelValue &val) // {{{
{ for (vector<Channel_MQ*>::const_iterator it=val.GetValues().begin();
       it!=val.GetValues().end();
       ++it)
    myChannels.push_back(new Channel_MQ((*it)->GetAddress()));
} // }}}
MQChannelValue &MQChannelValue::operator=(const MQChannelValue &rhs) // {{{
{ DeleteVector(myChannels);
  for (vector<Channel_MQ*>::const_iterator it=rhs.GetValues().begin();
       it!=rhs.GetValues().end();
       ++it)
    myChannels.push_back(new Channel_MQ((*it)->GetAddress()));
} // }}}
MQChannelValue *MQChannelValue::Copy() const // {{{
{
  return new MQChannelValue(*this);
} // }}}

vector<string> split(const std::string &s, char delim) // {{{
{
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
      result.push_back(item);
  }
  return result;
} // }}}
MQChannelValue::MQChannelValue(Message &val) // {{{
{ vector<string> addresses=split((string)val.GetData(),';');
  for (vector<string>::const_iterator it=addresses.begin();
       it!=addresses.end();
       ++it)
    myChannels.push_back(new Channel_MQ(*it));
} // }}}
MQChannelValue::MQChannelValue() // {{{
{
} // }}}
MQChannelValue::~MQChannelValue() // {{{
{ DeleteVector(myChannels);
} // }}}

string MQChannelValue::ToString() const // {{{
{ stringstream result;
  for (vector<Channel_MQ*>::const_iterator it=myChannels.begin();
       it!=myChannels.end();
       ++it)
  { if (it!=myChannels.begin())
      result << ";";
    result << (*it)->GetAddress();
  }
  return result.str();
} // }}}
bool MQChannelValue::operator==(const Value &rhs) const // {{{
{ const MQChannelValue *rhsptr=dynamic_cast<const MQChannelValue*>(&rhs);
  if (rhsptr==NULL)
    return false;
  if (myChannels.size()!=rhsptr->GetValues().size())
    return false;
  for (vector<Channel_MQ*>::const_iterator lhsCh=myChannels.begin(), rhsCh=rhsptr->GetValues().begin(); lhsCh!=myChannels.end() && rhsCh!=rhsptr->GetValues().end(); ++lhsCh, ++rhsCh)
    if ((*lhsCh)->GetAddress()!=(*rhsCh)->GetAddress())
      return false;
  return true;
} // }}}
const vector<Channel_MQ*> &MQChannelValue::GetValues() const // {{{
{ return myChannels;
} // }}}

// TupleValue Implementation
TupleValue::TupleValue(const TupleValue &val) // {{{
{ for (vector<Value*>::const_iterator it=val.GetValues().begin();
       it!=val.GetValues().end(); ++it)
    myValues.push_back((*it)->Copy());
} // }}}
TupleValue &TupleValue::operator=(const TupleValue &rhs) // {{{
{ DeleteVector(myValues);
  for (vector<Value*>::const_iterator it=rhs.GetValues().begin();
       it!=rhs.GetValues().end(); ++it)
    myValues.push_back((*it)->Copy());
} // }}}
TupleValue *TupleValue::Copy() const // {{{
{ return new TupleValue(*this);
} // }}}

//TupleValue::TupleValue(Message &msg) // {{{
//{ string msg_val=msg.GetData();
//  vector<string> vals=stringsplit(msg_val,"\\1");
//  for (vector<string>::const_iterator it=vals.begin(); it!=vals.end(); ++it)
//    myValues.push_back(Value::Create(stringreplace(*it,"\\2","\\")); // Add value to myValues
//} // }}}
TupleValue::TupleValue() // {{{
{
} // }}}
TupleValue::~TupleValue() // {{{
{ DeleteVector(myValues);
} // }}}

string TupleValue::ToString() const // {{{
{ stringstream result;
  for (vector<Value*>::const_iterator it=myValues.begin();
       it!=myValues.end(); ++it)
  { if (it!=myValues.begin())
      result << "\\1";
    result << stringreplace((*it)->ToString(),"\\","\\2");
  }
  return result.str();
} // }}}
const Value &TupleValue::GetValue(const IntValue &index) const // {{{
{ return GetValue(mpz_get_ui(index.GetValue()));
} // }}}
const Value &TupleValue::GetValue(int index) const // {{{
{ return *(myValues[index]);
} // }}}
bool TupleValue::operator==(const Value &rhs) const // {{{
{ const TupleValue *rhsptr=dynamic_cast<const TupleValue*>(&rhs);
  if (rhsptr==NULL)
    return false;
  if (rhsptr->GetValues().size()!=myValues.size())
    return false;
  for (int i=0; i<myValues.size(); ++i)
    if (!(GetValue(i)==rhsptr->GetValue(i)))
      return false;
  return true;
} // }}}
const vector<Value*> &TupleValue::GetValues() const // {{{
{ return myValues;
} // }}}
void TupleValue::AddValue(const Value &val) // {{{
{ myValues.push_back(val.Copy());
} // }}}

}
