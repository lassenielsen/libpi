#include <libpi/string.hpp>
#include <libpi/bool.hpp>
#include <sstream>
#include <b64/encode.h>
#include <b64/decode.h>

using namespace std;

namespace libpi
{

String::String(gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{
} // }}}

String::String(const string &val, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
, myValue(val)
{
} // }}}

String::String(const String &lhs, const String &rhs, String::STRINGOP op, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{ switch (op)
  { case OP_CONCAT:
      myValue=lhs.GetValue()+rhs.GetValue();
      break;
    default:
      throw string("Unknown String operation");
      break;
  }
} // }}}

String::~String() // {{{
{
} // }}}

void String::ToStream(ostream &dest) const // {{{
{ //stringstream ss;
  //ss << myValue;
  //base64::encoder enc;
  //enc.encode(ss,dest);
  // All this to get rid of a newline
  base64::encoder e;
  char* code = new char[2*myValue.length()];
  int codelength;

  codelength = e.encode(myValue.c_str(), myValue.length(), code);
  dest.write(code, codelength);

  codelength = e.encode_end(code);
  if (codelength>1)
    dest.write(code, codelength-1); // Skip newline
  delete [] code;
} // }}}

//String *String::operator+(const String &rhs) const // {{{
//{ return new String(myValue+rhs.GetValue());
//} // }}}
Bool *String::operator==(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue == rhsptr->GetValue());
} // }}}
Bool *String::operator<=(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue <= rhsptr->GetValue());
} // }}}
Bool *String::operator<(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue < rhsptr->GetValue());
} // }}}
Bool *String::operator>=(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue >= rhsptr->GetValue());
} // }}}
Bool *String::operator>(const Value &rhs) const // {{{
{ const String *rhsptr=dynamic_cast<const String*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  return Bool::GetInstance(myValue > rhsptr->GetValue());
} // }}}

Value *String::ParseString(istream &in, gc::GCRegistrant *registrant) // {{{
{ char delimiter=':';
  string str;
  std::getline(in,str,delimiter);
  stringstream enc;
  stringstream dec;
  enc << str;
  base64::decoder d;
  d.decode(enc,dec);
  return new String(dec.str(), registrant);
} // }}}

namespace stringvalue
{ int _init=Value::RegisterParser("str",String::ParseString);
}
}
