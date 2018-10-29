#include <libpi/tuple.hpp>
#include <libpi/bool.hpp>
#include <libpi/common.hpp>
#include <string>

using namespace std;

namespace libpi
{

Tuple::Tuple(gc::GCRegistrant *registrant) // {{{
: Value(registrant)
{
} // }}}
Tuple::Tuple(vector<Value*> &vals, gc::GCRegistrant *registrant) // {{{
: Value(registrant)
, myValues(vals)
{
} // }}}
Tuple::~Tuple() // {{{
{
} // }}}

void Tuple::ToStream(ostream &dest) const // {{{
{ dest << myValues.size();
  for (vector<Value*>::const_iterator it=myValues.begin();
       it!=myValues.end(); ++it)
  { dest << ":";
    (*it)->Serialize(dest);
  }
} // }}}
const Value *Tuple::GetValue(const Int &index) const // {{{
{ return GetValue(mpz_get_ui(index.GetValue()));
} // }}}
Value *Tuple::GetValue(const Int &index) // {{{
{ return GetValue(mpz_get_ui(index.GetValue()));
} // }}}
const Value *Tuple::GetValue(int index) const // {{{
{ return myValues[index];
} // }}}
Value *Tuple::GetValue(int index) // {{{
{ return myValues[index];
} // }}}
Bool *Tuple::operator==(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  if (rhsptr->GetValues().size()!=myValues.size())
    return Bool::GetInstance(false);
  for (int i=0; i<myValues.size(); ++i)
  { Bool *eltResult=(*GetValue(i))==(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
Bool *Tuple::operator<=(const Value &rhs) const // {{{
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
    Bool *eltResult=(*GetValue(i))<=(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
Bool *Tuple::operator<(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(false);
    if (i>=GetValues().size())
      return Bool::GetInstance(true);
    Bool *eltResult=(*GetValue(i))<(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
Bool *Tuple::operator>=(const Value &rhs) const // {{{
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
    Bool *eltResult=(*GetValue(i))>=(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
Bool *Tuple::operator>(const Value &rhs) const // {{{
{ const Tuple *rhsptr=dynamic_cast<const Tuple*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  for (int i=0; true; ++i)
  { if (i>=GetValues().size())
      return Bool::GetInstance(false);
    if (i>=rhsptr->GetValues().size())
      return Bool::GetInstance(true);
    Bool *eltResult=(*GetValue(i))>(*rhsptr->GetValue(i));
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
void Tuple::AddValue(Value *val) // {{{
{ myValues.push_back(val);
} // }}}
void Tuple::Mark(unordered_set<Value*> &marks) // {{{
{ if (marks.count(this)>0)
    return;
  
  marks.insert(this);
  for (vector<Value*>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    (*it)->Mark(marks);
} // }}}
Value *Tuple::ParseTuple(istream &in, gc::GCRegistrant *registrant) // {{{
{ char delimiter=':';
  string num_str;
  std::getline(in,num_str,delimiter);
  int num=stoi(num_str);
  
  vector<Value*> dst;
  for (int i=0; i<num; ++i)
    dst.push_back(Value::Parse(in,registrant));

  return new Tuple(dst,registrant);
} // }}}

namespace tuplevalue
{ int _init=Value::RegisterParser("tpl",Tuple::ParseTuple);
}
}
