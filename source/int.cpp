#include <libpi/int.hpp>
#include <libpi/bool.hpp>

using namespace std;

namespace libpi
{

// Int Implementation
Int::Int(const Int &val) // {{{
{ mpz_init(myValue);
  mpz_set(myValue,val.GetValue());
} // }}}
Int &Int::operator=(const Int &rhs) // {{{
{ mpz_set(myValue,rhs.GetValue());
  return *this;
} // }}}

Int::Int(const std::string &val) // {{{
{ mpz_init(myValue);
  int res=mpz_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad Int conversion of string: ") + val;
} // }}}
Int::Int(mpz_t &val, bool clear_arg) // {{{
{ mpz_init(myValue);
  mpz_set(myValue,val);
  if (clear_arg)
    mpz_clear(val);
} // }}}
Int::Int(long val) // {{{
{ mpz_init(myValue);
  mpz_set_si(myValue,val);
} // }}}

Int::~Int() // {{{
{ // Free value
  mpz_clear(myValue);
} // }}}

void Int::ToStream(ostream &dest) const // {{{
{ char *str=mpz_get_str(NULL,10,myValue);
  dest << string(str);
  free(str);
} // }}}
shared_ptr<Int> Int::operator+(const Int &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_add(res,myValue,rhs.GetValue());
  return shared_ptr<Int>(new Int(res,true));
} // }}}
shared_ptr<Int> Int::operator-(const Int &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_sub(res,myValue,rhs.GetValue());
  return shared_ptr<Int>(new Int(res,true));
} // }}}
shared_ptr<Int> Int::operator*(const Int &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_mul(res,myValue,rhs.GetValue());
  return shared_ptr<Int>(new Int(res,true));
} // }}}
shared_ptr<Int> Int::operator/(const Int &rhs) const // {{{
{ mpz_t res;
  mpz_init(res);
  mpz_tdiv_q(res,myValue,rhs.GetValue());
  return shared_ptr<Int>(new Int(res,true));
} // }}}
shared_ptr<Bool> Int::operator==(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp==0);
} // }}}
shared_ptr<Bool> Int::operator<=(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<=0);
} // }}}
shared_ptr<Bool> Int::operator<(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<0);
} // }}}
shared_ptr<Bool> Int::operator>=(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>=0);
} // }}}
shared_ptr<Bool> Int::operator>(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>0);
} // }}}
const mpz_t &Int::GetValue() const // {{{
{ return myValue;
} // }}}

shared_ptr<Value> Int::ParseInt(std::istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return shared_ptr<Int>(new Int(str));
} // }}}

namespace intvalue
{ int _init=Value::RegisterParser("int",Int::ParseInt);
}

}
