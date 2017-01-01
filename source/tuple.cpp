#include <libpi/tuple.hpp>
#include <libpi/bool.hpp>
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

void Tuple::ToStream(ostream &dest) const // {{{
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
shared_ptr<Bool> Tuple::operator==(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  if (rhsptr->GetValues().size()!=myValues.size())
    return Bool::GetInstance(false);
  for (int i=0; i<myValues.size(); ++i)
  { shared_ptr<Bool> eltResult=(*GetValue(i))==(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
shared_ptr<Bool> Tuple::operator<=(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=GetValues().size() && i>=rhsptr->GetValues().size())
      return Bool::GetInstance(true);
    if (i>=GetValues().size())
      return Bool::GetInstance(true);
    if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(false);
    shared_ptr<Bool> eltResult=(*GetValue(i))<=(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
shared_ptr<Bool> Tuple::operator<(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(false);
    if (i>=GetValues().size())
      return Bool::GetInstance(true);
    shared_ptr<Bool> eltResult=(*GetValue(i))<(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
shared_ptr<Bool> Tuple::operator>=(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=GetValues().size() && i>=rhsptr->GetValues().size())
      return Bool::GetInstance(true);
    if (i>=GetValues().size())
      return Bool::GetInstance(false);
    if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(true);
    shared_ptr<Bool> eltResult=(*GetValue(i))>=(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
shared_ptr<Bool> Tuple::operator>(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=GetValues().size())
      return Bool::GetInstance(false);
    if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(true);
    shared_ptr<Bool> eltResult=(*GetValue(i))>(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
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
