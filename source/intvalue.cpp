#include <libpi/intvalue.hpp>

using namespace std;

namespace libpi
{

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

IntValue::IntValue(const std::string &val) // {{{
{ mpz_init(myValue);
  int res=mpz_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad IntValue conversion of string: ") + val;
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
  if (rhsptr==NULL)
    return false;
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return cmp==0;
} // }}}
const mpz_t &IntValue::GetValue() const // {{{
{ return myValue;
} // }}}

Value *IntValue::ParseInt(const string &str) // {{{
{ return new IntValue(str);
} // }}}

namespace intvalue
{ int _init=Value::RegisterParser("int",IntValue::ParseInt);
}

}
