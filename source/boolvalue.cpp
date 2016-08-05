#include <libpi/boolvalue.hpp>

using namespace std;

namespace libpi
{

// BoolValue Implementation
BoolValue::BoolValue(const BoolValue &val) // {{{
{ myValue = val.GetValue();
} // }}}
BoolValue &BoolValue::operator=(const BoolValue &rhs) // {{{
{ myValue = rhs.GetValue();
} // }}}
BoolValue *BoolValue::Copy() const // {{{
{ return new BoolValue(*this);
} // }}}

BoolValue::BoolValue(const string &str) // {{{
{ if (str=="true")
    myValue = true;
  else if (str=="false")
    myValue=false;
  else throw (string)"BoolValue::BoolValue: Bad message value: " + str;
} // }}}
BoolValue::BoolValue(bool val) // {{{
{ myValue=val;
} // }}}
BoolValue::~BoolValue() // {{{
{
} // }}}

string BoolValue::ToString() const // {{{
{ if (myValue)
    return "true";
  else
    return "false";
} // }}}
BoolValue BoolValue::operator&&(const BoolValue &rhs) const // {{{
{ return BoolValue(GetValue() && rhs.GetValue());
} // }}}
BoolValue BoolValue::operator||(const BoolValue &rhs) const // {{{
{ return BoolValue(GetValue() || rhs.GetValue());
} // }}}
BoolValue BoolValue::operator!() const // {{{
{ return BoolValue(!GetValue());
} // }}}
bool BoolValue::operator==(const Value &rhs) const // {{{
{ const BoolValue *rhsptr=dynamic_cast<const BoolValue*>(&rhs);
  if (rhsptr==NULL)
    return false;
  return myValue==rhsptr->GetValue();
} // }}}

bool BoolValue::GetValue() const // {{{
{ return myValue;
} // }}}

Value *BoolValue::ParseBool(const string &str) // {{{
{ return new BoolValue(str);
} // }}}

namespace boolvalue
{ int _init=Value::RegisterParser("boo",&BoolValue::ParseBool);
}
}
