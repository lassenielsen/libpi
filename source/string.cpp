#include <libpi/string.hpp>

using namespace std;

namespace libpi
{

String::String() // {{{
{
} // }}}

String::String(const string &val) // {{{
: myValue(val)
{
} // }}}

String::~String() // {{{
{
} // }}}

void String::ToString(ostream &dest) const // {{{
{ dest << myValue;
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

Value *String::ParseString(istream &in) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  return new String(str);
} // }}}

namespace stringvalue
{ int _init=Value::RegisterParser("str",String::ParseString);
}
}
