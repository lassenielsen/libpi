#include <libpi/tuple.hpp>
#include <libpi/common.hpp>
#include <string>

using namespace std;

namespace libpi
{

Tuple::Tuple() // {{{
{
} // }}}
Tuple::Tuple(vector<shared_ptr<Value> > &vals) // {{{
: myValues(vals)
{
} // }}}
Tuple::~Tuple() // {{{
{
} // }}}

void Tuple::ToString(ostream &dest) const // {{{
{ dest << myValues.size();
  for (vector<shared_ptr<Value> >::const_iterator it=myValues.begin();
       it!=myValues.end(); ++it)
  { dest << ":";
    (*it)->Serialize(dest);
  }
} // }}}
const shared_ptr<Value> &Tuple::GetValue(const Int &index) const // {{{
{ return GetValue(mpz_get_ui(index.GetValue()));
} // }}}
shared_ptr<Value> &Tuple::GetValue(const Int &index) // {{{
{ return GetValue(mpz_get_ui(index.GetValue()));
} // }}}
const shared_ptr<Value> &Tuple::GetValue(int index) const // {{{
{ return myValues[index];
} // }}}
shared_ptr<Value> &Tuple::GetValue(int index) // {{{
{ return myValues[index];
} // }}}
bool Tuple::operator==(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return false;
  if (rhsptr->GetValues().size()!=myValues.size())
    return false;
  for (int i=0; i<myValues.size(); ++i)
    if (!(GetValue(i)==rhsptr->GetValue(i)))
      return false;
  return true;
} // }}}
const vector<shared_ptr<Value> > &Tuple::GetValues() const // {{{
{ return myValues;
} // }}}
vector<shared_ptr<Value> > &Tuple::GetValues() // {{{
{ return myValues;
} // }}}
void Tuple::AddValue(shared_ptr<Value> val) // {{{
{ myValues.push_back(val);
} // }}}
Value *Tuple::ParseTuple(istream &in) // {{{
{ char delimiter=':';
  string num_str;
  std::getline(in,num_str,delimiter);
  int num=stoi(num_str);
  
  vector<shared_ptr<Value> > dst;
  for (int i=0; i<num; ++i)
    dst.push_back(shared_ptr<Value>(Value::Parse(in)));

  return new Tuple(dst);
} // }}}

namespace tuplevalue
{ int _init=Value::RegisterParser("tpl",Tuple::ParseTuple);
}
}
