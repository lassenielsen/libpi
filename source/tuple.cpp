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
Tuple::Tuple(vector<Value*> &vals) // {{{
{ for (size_t i=0; i<vals.size(); ++i)
  { vals[i]->AddRef();
    myValues.push_back(vals[i]);
  }
} // }}}
Tuple::~Tuple() // {{{
{ while (!myValues.empty())
  { myValues.back()->RemoveRef();
    myValues.pop_back();
  }
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
      return eltResult; // return false but simpler
    else
      eltResult->RemoveRef();
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
      return eltResult; // return false but simpler
    else
      eltResult->RemoveRef();
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
      return eltResult; // return false but simpler
    else
      eltResult->RemoveRef();
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
      return eltResult; // return false but simpler
    else
      eltResult->RemoveRef();
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
      return eltResult; // return false but simpler
    else
      eltResult->RemoveRef();
  }
  return Bool::GetInstance(true);
} // }}}
void Tuple::AddValue(Value *val) // {{{
{ val->AddRef();
  myValues.push_back(val);
} // }}}
Value *Tuple::ParseTuple(istream &in) // {{{
{ char delimiter=':';
  string num_str;
  std::getline(in,num_str,delimiter);
  int num=stoi(num_str);
  
  vector<Value*> dst;
  for (int i=0; i<num; ++i)
    dst.push_back(Value::Parse(in));

  return new Tuple(dst);
} // }}}

namespace tuplevalue
{ int _init=Value::RegisterParser("tpl",Tuple::ParseTuple);
}
}
