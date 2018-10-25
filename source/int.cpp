#include <libpi/int.hpp>
#include <libpi/bool.hpp>

using namespace std;

namespace libpi
{

// Int Implementation
Int::Int(const Int &val, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{ mpz_init(myValue);
  mpz_set(myValue,val.GetValue());
} // }}}
Int::Int(const Int &lhs, const Int &rhs, Int::INTOP op, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{ mpz_init(myValue);
  switch (op)
  { case OP_ADD:
      mpz_add(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    case OP_SUB:
      mpz_sub(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    case OP_MULT:
      mpz_mul(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    case OP_DIV:
      mpz_tdiv_q(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    default:
      throw string("Unknown Int opreration");
      break;
  }
} // }}}
Int &Int::operator=(const Int &rhs) // {{{
{ mpz_set(myValue,rhs.GetValue());
} // }}}

Int::Int(const std::string &val, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{ mpz_init(myValue);
  int res=mpz_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad Int conversion of string: ") + val;
} // }}}
Int::Int(mpz_t &val, gc::GCRegistrant *registrant, bool clear_arg) // {{{
: Value(registrant)
{ mpz_init(myValue);
  mpz_set(myValue,val);
  if (clear_arg)
    mpz_clear(val);
} // }}}
Int::Int(long val, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
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
//Int *Int::operator+(const Int &rhs) const // {{{
//{ mpz_t res;
//  mpz_init(res);
//  mpz_add(res,myValue,rhs.GetValue());
//  return new Int(res,true);
//} // }}}
//Int *Int::operator-(const Int &rhs) const // {{{
//{ mpz_t res;
//  mpz_init(res);
//  mpz_sub(res,myValue,rhs.GetValue());
//  return new Int(res,true);
//} // }}}
//Int *Int::operator*(const Int &rhs) const // {{{
//{ mpz_t res;
//  mpz_init(res);
//  mpz_mul(res,myValue,rhs.GetValue());
//  return new Int(res,true);
//} // }}}
//Int *Int::operator/(const Int &rhs) const // {{{
//{ mpz_t res;
//  mpz_init(res);
//  mpz_tdiv_q(res,myValue,rhs.GetValue());
//  return new Int(res,true);
//} // }}}
Bool *Int::operator==(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp==0);
} // }}}
Bool *Int::operator<=(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<=0);
} // }}}
Bool *Int::operator<(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<0);
} // }}}
Bool *Int::operator>=(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>=0);
} // }}}
Bool *Int::operator>(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpz_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>0);
} // }}}
const mpz_t &Int::GetValue() const // {{{
{ return myValue;
} // }}}

Value *Int::ParseInt(std::istream &in, gc::GCRegistrant *registrant) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return new Int(str,registrant);
} // }}}

namespace intvalue
{ int _init=Value::RegisterParser("int",Int::ParseInt);
}

}
