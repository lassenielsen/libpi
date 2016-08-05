#include <libpi/stringvalue.hpp>

using namespace std;

namespace libpi
{

// StringValue Implementation
StringValue::StringValue(const StringValue &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}
StringValue &StringValue::operator=(const StringValue &rhs) // {{{
{ myValue=rhs.GetValue();
} // }}}
StringValue *StringValue::Copy() const // {{{
{ return new StringValue(*this);
} // }}}

StringValue::StringValue(const std::string &val) // {{{
{ myValue=val;
} // }}}
StringValue::StringValue() // {{{
{ myValue="";
} // }}}

StringValue::~StringValue() // {{{
{
} // }}}

string StringValue::ToString() const // {{{
{ return myValue;
} // }}}

StringValue StringValue::operator+(const StringValue &rhs) const // {{{
{ return StringValue(myValue+rhs.GetValue());
} // }}}
bool StringValue::operator==(const Value &rhs) const // {{{
{ const StringValue *rhsptr=dynamic_cast<const StringValue*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue == rhsptr->GetValue();
} // }}}
const std::string &StringValue::GetValue() const // {{{
{ return myValue;
} // }}}

Value *StringValue::ParseString(const string &str) // {{{
{ return new StringValue(str);
} // }}}

namespace stringvalue
{ int _init=Value::RegisterParser("str",StringValue::ParseString);
}
}
