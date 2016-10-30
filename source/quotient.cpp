#include <libpi/quotient.hpp>
#include <iostream>

using namespace std;

namespace libpi
{

// Quotient Implementation
Quotient::Quotient(const Quotient &val) // {{{
{ mpq_init(myValue);
  mpq_set(myValue,val.GetValue());
} // }}}
Quotient &Quotient::operator=(const Quotient &rhs) // {{{
{ mpq_set(myValue,rhs.GetValue());
} // }}}

Quotient::Quotient() // {{{
{ mpq_init(myValue);
} // }}}
Quotient::Quotient(const std::string &val) // {{{
{ mpq_init(myValue);
  size_t pos=val.find('.');
  if (pos!=string::npos)
  { string ipart=val.substr(0,pos);
    string fpart=val.substr(pos+1);
    size_t denomexp=fpart.size();
    string fval=ipart+fpart+"/1";
    for (size_t i=0; i<denomexp; ++i)
      fval+="0";
    int res=mpq_set_str(myValue,fval.c_str(),10);
    if (res<0) throw string("Bad Quotient conversion of float string: ") + val;
  }
  else
  { int res=mpq_set_str(myValue,val.c_str(),10);
    if (res<0) throw string("Bad Quotient conversion of fraction string: ") + val;
  }
  mpq_canonicalize(myValue);
} // }}}
Quotient::Quotient(mpq_t &val, bool clear_arg) // {{{
{ mpq_init(myValue);
  mpq_set(myValue,val);
  if (clear_arg)
    mpq_clear(val);
} // }}}
Quotient::Quotient(double val) // {{{
{ mpf_t tmp;
  mpf_init(tmp);
  mpf_set_d(tmp,val);
  mpq_init(myValue);
  mpq_set_f(myValue,tmp);
  mpf_clear(tmp);
  mpq_canonicalize(myValue);
} // }}}

Quotient::~Quotient() // {{{
{ // Free value
  mpq_clear(myValue);
} // }}}

void Quotient::ToString(ostream &dest) const // {{{
{ char *str=mpq_get_str(NULL,10,myValue);
  dest << str;
  free(str);
} // }}}
shared_ptr<Quotient> Quotient::operator+(const Quotient &rhs) const // {{{
{ shared_ptr<Quotient> res=shared_ptr<Quotient>(new Quotient(false));
  mpq_add(res->GetValue(),GetValue(),rhs.GetValue());
  return res;
} // }}}
shared_ptr<Quotient> Quotient::operator-(const Quotient &rhs) const // {{{
{ shared_ptr<Quotient> res=shared_ptr<Quotient>(new Quotient(false));
  mpq_sub(res->GetValue(),GetValue(),rhs.GetValue());
  return res;
} // }}}
shared_ptr<Quotient> Quotient::operator*(const Quotient &rhs) const // {{{
{ shared_ptr<Quotient> res=shared_ptr<Quotient>(new Quotient(false));
  mpq_mul(res->GetValue(),GetValue(),rhs.GetValue());
  return res;
} // }}}
shared_ptr<Quotient> Quotient::operator/(const Quotient &rhs) const // {{{
{ shared_ptr<Quotient> res=shared_ptr<Quotient>(new Quotient(false));
  mpq_div(res->GetValue(),GetValue(),rhs.GetValue());
  return res;
} // }}}
bool Quotient::operator<=(const Quotient &rhs) const // {{{
{ int cmp = mpq_cmp(GetValue(),rhs.GetValue());
  return cmp<=0;
} // }}}
bool Quotient::operator==(const Value &rhs) const // {{{
{ const Quotient *rhsptr=dynamic_cast<const Quotient*>(&rhs);
  if (rhsptr==NULL)
    return false;
  int cmp = mpq_cmp(GetValue(),rhsptr->GetValue());
  return cmp==0;
} // }}}

Value *Quotient::ParseQuotient(std::istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return new Quotient(str);
} // }}}

namespace quotientvalue
{ int _init=Value::RegisterParser("qrt",Quotient::ParseQuotient);
}

}