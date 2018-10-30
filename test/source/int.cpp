#include "worker.hpp"
#include <libpi/int.hpp>
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
  { Int x0("1",NULL);
    cout << "x0=" << x0.ToString() << endl;
 
    Int x1(1,NULL);
    Compare(x1.Serialize(),"int:1","Serialize");
    { Value *parsed=Value::Parse("int:1",NULL);
      Compare(parsed->Serialize(),"int:1","Parse");
    }
    Int x2(x1,x1,Int::OP_ADD,NULL);
    Compare(x2.Serialize(),"int:2","Sum");
    Int x4(x2,x2,Int::OP_MULT,NULL);
    Compare(x4.Serialize(),"int:4","Product");
    Int x3(x4,x1,Int::OP_SUB,NULL);
    Compare(x3.Serialize(),"int:3","Difference");
    Int y2(x4,x2,Int::OP_DIV,NULL);
    Compare(y2.Serialize(),"int:2","Quotient");
    Compare((x1<=x2)->GetValue(),true,"LEQ");
    Compare((x3<=x2)->GetValue(),false,"LEQ");
    Compare((x2==y2)->GetValue(),true,"EQ");
    Compare((x3==y2)->GetValue(),false,"EQ");
    Compare((y2==x3)->GetValue(),false,"EQ");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
