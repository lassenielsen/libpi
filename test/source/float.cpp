#include "worker.hpp"
#include <libpi/float.hpp>
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
  { Float *x0(new Float(3.1415,NULL));
    Compare(x0->Serialize().substr(0,10),"flt:3.1415","Serialize");
    Float *x1(new Float("3.1415",NULL));
    Compare(x1->Serialize(),"flt:3.1415","Initialize");
    { Value *parsed=Value::Parse("flt:3.1415",NULL);
      Compare(parsed->Serialize(),"flt:3.1415","Parse");
    }
    Float *x2(new Float(*x1,*x1,Float::OP_ADD,NULL));
    Compare(x2->Serialize().substr(0,10),"flt:6.2829","Sum");
    Float *x4(new Float(*x2,*x2,Float::OP_MULT,NULL));
    Compare(x4->Serialize().substr(0,14),"flt:39.4760889","Product");
    Float *x3(new Float(*x4,*x1,Float::OP_SUB,NULL));
    Compare(x3->Serialize().substr(0,14),"flt:36.3345889","Difference");
    Float *y2(new Float(*x4,*x2,Float::OP_DIV,NULL));
    Compare(y2->Serialize().substr(0,10),"flt:6.2829","Quotient");
    Compare(((*x1)<=(*x2))->GetValue(),true,"LEQ");
    Compare(((*x3)<=(*x2))->GetValue(),false,"LEQ");
    //Error due to inaccuracy Compare(((*x2)==(*y2))->GetValue(),true,"EQ");
    Compare(((*x3)==(*y2))->GetValue(),false,"EQ");
    Compare(((*y2)==(*x3))->GetValue(),false,"EQ");
    Float *x5(new Float("5.0",NULL));
    Compare(x5->Serialize(),"flt:5.0","Serialize");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
