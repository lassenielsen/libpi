#include <libpi/int.hpp>
#include <libpi/bool.hpp>
#include <sstream>

using namespace std;
using namespace NTL;

namespace libpi
{

// Int Implementation
Int::Int(const Int &val) // {{{
{ myValue=val.GetValue();
} // }}}
Int &Int::operator=(const Int &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}

Int::Int(const std::string &val) // {{{
{ stringstream ss;
  ss << val;
  ss >> myValue;
} // }}}
Int::Int(ZZ &val) // {{{
{ myValue=val;
} // }}}
Int::Int(long val) // {{{
{ myValue=val;
} // }}}

Int::~Int() // {{{
{
} // }}}

void Int::ToStream(ostream &dest) const // {{{
{ dest << myValue;
} // }}}
shared_ptr<Int> Int::operator+(const Int &rhs) const // {{{
{ ZZ res(myValue + rhs.GetValue());
  return shared_ptr<Int>(new Int(res));
} // }}}
shared_ptr<Int> Int::operator-(const Int &rhs) const // {{{
{ ZZ res(myValue - rhs.GetValue());
  return shared_ptr<Int>(new Int(res));
} // }}}
shared_ptr<Int> Int::operator*(const Int &rhs) const // {{{
{ ZZ res(myValue * rhs.GetValue());
  return shared_ptr<Int>(new Int(res));
} // }}}
shared_ptr<Int> Int::operator/(const Int &rhs) const // {{{
{ ZZ res(myValue / rhs.GetValue());
  return shared_ptr<Int>(new Int(res));
} // }}}
shared_ptr<Bool> Int::operator==(const Value &rhs) const // {{{
{ const Int* rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue == rhsptr->GetValue());
} // }}}
shared_ptr<Bool> Int::operator<=(const Value &rhs) const // {{{
{ const Int *rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue <= rhsptr->GetValue());
} // }}}
shared_ptr<Bool> Int::operator<(const Value &rhs) const // {{{
{ const Int* rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue < rhsptr->GetValue());
} // }}}
shared_ptr<Bool> Int::operator>=(const Value &rhs) const // {{{
{ const Int* rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue >= rhsptr->GetValue());
} // }}}
shared_ptr<Bool> Int::operator>(const Value &rhs) const // {{{
{ const Int* rhsptr=dynamic_cast<const Int*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue > rhsptr->GetValue());
} // }}}
const ZZ &Int::GetValue() const // {{{
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
