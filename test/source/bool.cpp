#include "worker.hpp"
#include <libpi/bool.hpp>
#include <iostream>
#include <sstream>

using namespace libpi;
using namespace std;

void Compare(const string &lhs, const string &rhs, const string name) // {{{
{ cout << "Testing " << name << ": ";
  if (lhs!=rhs)
  { cout << "FAILED" << endl;
    throw string("Error in ")+name+" - "+lhs+" != "+rhs;
  }
  else
    cout << "SUCCESS" << endl;
} // }}}

void Compare(bool lhs, bool rhs, const string name) // {{{
{ return Compare(string(lhs?"true":"false"),string(rhs?"true":"false"),name);
} // }}}

int main(int argc, char **argv)
{ try
  { Bool *b0=Bool::GetInstance(false);
    Compare(b0->Serialize(),"boo:false","Serialize");
    Bool *b1=Bool::GetInstance(true);
    Compare(b1->Serialize(),"boo:true","Serialize");
    Compare((!(*b0))->Serialize(),"boo:true","Negation");
    Compare((!(*b1))->Serialize(),"boo:false","Negation");
    Compare(((*b1)&&(*b1))->Serialize(),"boo:true","Conjunction");
    Compare(((*b0)&&(*b1))->Serialize(),"boo:false","Conjunction");
    Compare(((*b1)&&(*b0))->Serialize(),"boo:false","Conjunction");
    Compare(((*b0)&&(*b0))->Serialize(),"boo:false","Conjunction");
    Compare(((*b1)||(*b1))->Serialize(),"boo:true","Disjunction");
    Compare(((*b0)||(*b1))->Serialize(),"boo:true","Disjunction");
    Compare(((*b1)||(*b0))->Serialize(),"boo:true","Disjunction");
    Compare(((*b0)||(*b0))->Serialize(),"boo:false","Disjunction");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
