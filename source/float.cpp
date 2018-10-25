#include <libpi/float.hpp>
#include <libpi/bool.hpp>
#include <string.h>

using namespace std;

namespace libpi
{

// Float Implementation
Float::Float(const Float &val, gc::Registrant *registrant) // {{{
: Value(registrant)
{ mpf_init2(myValue,256);
  mpf_set(myValue,val.GetValue());
} // }}}
Float::Float(const Float &lhs, const Float &rhs, Float::FLOATOP op, gc::Registrant *registrant) // {{{
: Value(registrant)
{ mpf_init2(myValue,256);
  switch (op)
  { case OP_ADD:
      mpf_add(myValie,lhs.getValue(),rhs.GetValue());
      break;
    case OP_SUB:
      mpf_sub(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    case OP_MULT:
      mpf_mul(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    case OP_DIV:
      mpf_div(myValue,lhs.GetValue(),rhs.GetValue());
      break;
    default:
      throw string("Unknown Int opreration");
      break;
  }
} // }}}
Float &Float::operator=(const Float &rhs) // {{{
{ mpf_set(myValue,rhs.GetValue());
} // }}}

Float::Float(const std::string &val, gc::Registrant *registrant) // {{{
: Value(registrant)
{ mpf_init2(myValue,256);
  int res=mpf_set_str(myValue,val.c_str(),10);
  if (res<0) throw string("Bad Float conversion of string: ") + val;
} // }}}
Float::Float(mpf_t &val, gc::Registrant *registrant, bool clear_arg) // {{{
: Value(registrant)
{ mpf_init2(myValue,256);
  mpf_set(myValue,val);
  if (clear_arg)
    mpf_clear(val);
} // }}}
Float::Float(double val, gc::Registrant *registrant) // {{{
: Value(registrant)
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
  if (exp>=strlen(str))
    dest << str << string(exp-strlen(str),'0') << ".0";
  else if (exp>=0)
    dest << string(str,exp) << "." << string(str+exp);
  else
    dest << "0." << string(1-exp,'0') << string(str);
  free(str);
} // }}}
//Float *Float::operator+(const Float &rhs) const // {{{
//{ mpf_t res;
//  mpf_init2(res,256);
//  mpf_add(res,myValue,rhs.GetValue());
//  return new Float(res,true);
//} // }}}
//Float *Float::operator-(const Float &rhs) const // {{{
//{ mpf_t res;
//  mpf_init2(res,256);
//  mpf_sub(res,myValue,rhs.GetValue());
//  return new Float(res,true);
//} // }}}
//Float *Float::operator*(const Float &rhs) const // {{{
//{ mpf_t res;
//  mpf_init2(res,256);
//  mpf_mul(res,myValue,rhs.GetValue());
//  return new Float(res,true);
//} // }}}
//Float *Float::operator/(const Float &rhs) const // {{{
//{ mpf_t res;
//  mpf_init2(res,256);
//  mpf_div(res,myValue,rhs.GetValue());
//  return new Float(res,true);
//} // }}}
Bool *Float::operator==(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp==0);
} // }}}
Bool *Float::operator<=(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<=0);
} // }}}
Bool *Float::operator<(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp<0);
} // }}}
Bool *Float::operator>=(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>=0);
} // }}}
Bool *Float::operator>(const Value &rhs) const // {{{
{ const Float *rhsptr=dynamic_cast<const Float*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  int cmp = mpf_cmp(myValue,rhsptr->GetValue());
  return Bool::GetInstance(cmp>0);
} // }}}

Value *Float::ParseFloat(std::istream &in, gc::Registrant *registrant) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return new Float(str,registrant);
} // }}}

namespace floatvalue
{ int _init=Value::RegisterParser("flt",Float::ParseFloat);
}

}
