#include <libpi/float.hpp>

using namespace std;

namespace libpi
{

// Float Implementation
Float::Float(const Float &val) // {{{
{ mpf_init(myValue);
  mpf_set(myValue,val.GetValue());
} // }}}
Float &Float::operator=(const Float &rhs) // {{{
{ mpf_set(myValue,rhs.GetValue());
} // }}}

Float::Float(const std::string &val) // {{{
{ mpf_init(myValue);
  int res=mpf_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad Float conversion of string: ") + val;
} // }}}
Float::Float(mpf_t &val, bool clear_arg) // {{{
{ mpf_init(myValue);
  mpf_set(myValue,val);
  if (clear_arg)
    mpf_clear(val);
} // }}}
Float::Float(double val) // {{{
{ mpf_init(myValue);
  mpf_set_d(myValue,val);
} // }}}

Float::~Float() // {{{
{ // Free value
  mpf_clear(myValue);
} // }}}

void Float::ToString(ostream &dest) const // {{{
{ mp_exp_t exp;
  char *str=mpf_get_str(NULL,&exp,10,0,myValue);
  if (exp>0)
    dest << string(str,exp) << "." << string(str+exp);
  else
    dest << "0." << string(-exp,'0') << string(str);
} // }}}
shared_ptr<Float> Float::operator+(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init(res);
  mpf_add(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator-(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init(res);
  mpf_sub(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator*(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init(res);
  mpf_mul(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
shared_ptr<Float> Float::operator/(const Float &rhs) const // {{{
{ mpf_t res;
  mpf_init(res);
  mpf_div(res,myValue,rhs.GetValue());
  return shared_ptr<Float>(new Float(res,true));
} // }}}
bool Float::operator<=(const Float &rhs) const // {{{
{ int cmp = mpf_cmp(myValue,rhs.GetValue());
  return cmp<=0;
} // }}}
bool Float::operator==(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return false;
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return cmp==0;
} // }}}
const mpf_t &Float::GetValue() const // {{{
{ return myValue;
} // }}}

Value *Float::ParseFloat(std::istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return new Float(str);
} // }}}

namespace floatvalue
{ int _init=Value::RegisterParser("flt",Float::ParseFloat);
}

}
