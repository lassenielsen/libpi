#include <libpi/string.hpp>

using namespace std;

namespace libpi
{

// String Implementation
String::String(const String &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}
String &String::operator=(const String &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}

String::String(const std::string &val) // {{{
{ myValue=val;
} // }}}
String::String() // {{{
{ myValue="";
} // }}}

String::~String() // {{{
{
} // }}}

string String::ToString() const // {{{
{ return myValue;
} // }}}

String String::operator+(const String &rhs) const // {{{
{ return String(myValue+rhs.GetValue());
} // }}}
bool String::operator==(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue == rhsptr->GetValue();
} // }}}
const std::string &String::GetValue() const // {{{
{ return myValue;
} // }}}

Value *String::ParseString(const string &str) // {{{
{ return new String(str);
} // }}}

namespace stringvalue
{ int _init=Value::RegisterParser("str",String::ParseString);
}
}
