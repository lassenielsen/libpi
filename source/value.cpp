#include <libpi/value.hpp>
#include <string.h>

using namespace std;
using namespace libpi;

// Value (partial) Implementation
Value::~Value // {{{
{
} // }}}

// IntValue Implementation
IntValue::IntValue(const IntValue &rhs) // {{{
{ mpz_init(myValue);
  mpz_set(myValue,rhs.GetValue());
} // }}}
IntValue &IntValue::operator=(const IntValue &rhs) // {{{
{ mpz_set(myValue,rhs.GetValue());
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

void IntValue::ToMessage(Message &msg) // {{{
{ char *data=mpz_get_str(NULL,10,myValue);
  msg.AddData(data,strlen(data)+1);
} // }}}

IntValue IntValue::operator+(const IntValue &rhs) // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_add(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator-(const IntValue &rhs) // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_sub(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator*(const IntValue &rhs) // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_mul(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
IntValue IntValue::operator/(const IntValue &rhs) // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_tdiv_q(res,myValue,rhs.GetValue());
  return IntValue(res,true);
} // }}}
bool IntValue::operator<=(const IntValue &rhs) // {{{
{ int cmp = mpz_cmp(myValue,rhs.GetValue());
  return cmp<=0;
} // }}}
bool IntValue::operator==(const IntValue &rhs) // {{{
{ int cmp = mpz_cmp(myValue,rhs.GetValue());
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

StringValue::StringValue(Message &msg) // {{{
{ myValue=msg.GetData();
} // }}}
StringValue::StringValue(const std::string &val) // {{{
{ myValue=val;
} // }}}

StringValue::~StringValue() // {{{
{
} // }}}

void StringValue::ToMessage(Message &msg) // {{{
{ msg.AddData(myValue.c_str(),myValue.size()+1);
} // }}}

StringValue StringValue::operator+(const StringValue &rhs) // {{{
{ return StringValue(myValue+rhs.GetValue());
} // }}}
bool StringValue::operator==(const StringValue &rhs) // {{{
{ return myValue == rhs.GetValue();
} // }}}
const std::string &StringValue::GetValue() const // {{{
{ return myValue;
} // }}}

// BoolValue Implementation

// TupleValue Implementation

