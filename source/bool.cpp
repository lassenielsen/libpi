#include <libpi/bool.hpp>

using namespace std;

namespace libpi
{

// Bool Implementation
Bool::Bool(const Bool &val) // {{{
{ myValue = val.GetValue();
} // }}}

Bool::Bool(bool val) // {{{
{ myValue=val;
} // }}}
Bool::~Bool() // {{{
{
} // }}}

void Bool::ToStream(ostream &dest) const // {{{
{ if (myValue)
    dest << "true";
  else
    dest << "false";
} // }}}
bool Bool::operator&&(const Bool &rhs) const // {{{
{ return GetValue() && rhs.GetValue();
} // }}}
bool Bool::operator||(const Bool &rhs) const // {{{
{ return GetValue() || rhs.GetValue();
} // }}}
bool Bool::operator!() const // {{{
{ return !GetValue();
} // }}}
bool Bool::operator==(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue==rhsptr->GetValue();
} // }}}
bool Bool::operator<=(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return rhsptr->GetValue() || !myValue;
} // }}}
bool Bool::operator<(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return rhsptr->GetValue() && !myValue;
} // }}}
bool Bool::operator>=(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue || !rhsptr->GetValue();
} // }}}
bool Bool::operator>(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue && !rhsptr->GetValue();
} // }}}

Value* Bool::ParseBool(istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return GetInstance(str);
} // }}}

Bool* Bool::trueInstance(new Bool(true));
Bool* Bool::falseInstance(new Bool(false));

namespace boolvalue
{ int _init=Value::RegisterParser("boo",&Bool::ParseBool);
}
}
