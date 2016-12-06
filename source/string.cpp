#include <libpi/string.hpp>
#include <sstream>
#include <b64/encode.h>
#include <b64/decode.h>

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

shared_ptr<String> String::operator+(const String &rhs) const // {{{
{ return shared_ptr<String>(new String(myValue+rhs.GetValue()));
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
  stringstream enc;
  stringstream dec;
  enc << str;
  base64::decoder d;
  d.decode(enc,dec);
  return new String(dec.str());
} // }}}

namespace stringvalue
{ int _init=Value::RegisterParser("str",String::ParseString);
}
}
