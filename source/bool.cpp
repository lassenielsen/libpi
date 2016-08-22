#include <libpi/bool.hpp>

using namespace std;

namespace libpi
{

// Bool Implementation
Bool::Bool(const Bool &val) // {{{
{ myValue = val.GetValue();
} // }}}
Bool &Bool::operator=(const Bool &rhs) // {{{
{ myValue = rhs.GetValue();
} // }}}

Bool::Bool(const string &str) // {{{
{ if (str=="true")
    myValue = true;
  else if (str=="false")
    myValue=false;
  else throw (string)"Bool::Bool: Bad message value: " + str;
} // }}}
Bool::Bool(bool val) // {{{
{ myValue=val;
} // }}}
Bool::~Bool() // {{{
{
} // }}}

string Bool::ToString() const // {{{
{ if (myValue)
    return "true";
  else
    return "false";
} // }}}
Bool Bool::operator&&(const Bool &rhs) const // {{{
{ return Bool(GetValue() && rhs.GetValue());
} // }}}
Bool Bool::operator||(const Bool &rhs) const // {{{
{ return Bool(GetValue() || rhs.GetValue());
} // }}}
Bool Bool::operator!() const // {{{
{ return Bool(!GetValue());
} // }}}
bool Bool::operator==(const Value &rhs) const // {{{
{ const Bool *rhsptr=dynamic_cast<const Bool*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue==rhsptr->GetValue();
} // }}}

bool Bool::GetValue() const // {{{
{ return myValue;
} // }}}

Value *Bool::ParseBool(const string &str) // {{{
{ return new Bool(str);
} // }}}

namespace boolvalue
{ int _init=Value::RegisterParser("boo",&Bool::ParseBool);
}
}
