#include <libpi/float.hpp>
#include <libpi/bool.hpp>
#include <string.h>

using namespace std;

namespace libpi
{

// Float Implementation
Float::Float(const Float &val) // {{{
{ mpf_init2(myValue,256);
  mpf_set(myValue,val.GetValue());
} // }}}
Float &Float::operator=(const Float &rhs) // {{{
{ mpf_set(myValue,rhs.GetValue());
  return *this;
} // }}}

Float::Float(const std::string &val) // {{{
{ int res=mpf_init_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad Float conversion of string: ") + val;
} // }}}
Float::Float(mpf_t &val, bool clear_arg) // {{{
{ mpf_init2(myValue,256);
  mpf_set(myValue,val);
  if (clear_arg)
    mpf_clear(val);
} // }}}
Float::Float(mpz_t &val, bool clear_arg) // {{{
{ mpf_init2(myValue,256);
  mpf_set_z(myValue,val);
  if (clear_arg)
    mpz_clear(val);
} // }}}
Float::Float(double val) // {{{
{ mpf_init2(myValue,256);
  mpf_set_d(myValue,val);
} // }}}

Float::~Float() // {{{
{ // Free value
  mpf_clear(myValue);
} // }}}

void Float::ToStream(ostream &dest) const // {{{
{ mp_exp_t exp;
  char *str=mpf_get_str(NULL,&exp,10,0,myValue);
  char *spos=str;
  if (spos[0]=='-')
  { dest << spos[0];
    ++spos;
  }
  if (int(exp)>=int(strlen(spos)))
    dest << spos << string(int(exp)-strlen(spos),'0') << ".0";
  else if (int(exp)>0)
    dest << (string(spos,int(exp))) << "." << string(spos+int(exp));
  else
    dest << "0." << string(-int(exp),'0') << string(spos);
  free(str);
} // }}}
shared_ptr<Float> Float::operator+(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init2(res,256);
  mpf_add(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator-(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init2(res,256);
  mpf_sub(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator*(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init2(res,256);
  mpf_mul(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator/(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init2(res,256);
  mpf_div(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Bool> Float::operator==(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp==0);
} // }}}
shared_ptr<Bool> Float::operator<=(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<=0);
} // }}}
shared_ptr<Bool> Float::operator<(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<0);
} // }}}
shared_ptr<Bool> Float::operator>=(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>=0);
} // }}}
shared_ptr<Bool> Float::operator>(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>0);
} // }}}

shared_ptr<Value> Float::ParseFloat(std::istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return shared_ptr<Float>(new Float(str));
} // }}}

namespace floatvalue
{ int _init=Value::RegisterParser("flt",Float::ParseFloat);
}

}
