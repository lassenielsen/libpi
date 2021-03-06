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
  { shared_ptr<Int> x0(new Int("1"));
    cout << "x0=" << x0->ToString() << endl;
 
    shared_ptr<Int> x1(new Int(1));
    Compare(x1->Serialize(),"int:1","Serialize");
    { shared_ptr<Value> parsed=shared_ptr<Value>(Value::Parse("int:1"));
      Compare(parsed->Serialize(),"int:1","Parse");
    }
    shared_ptr<Int> x2=(*x1)+(*x1);
    Compare(x2->Serialize(),"int:2","Sum");
    shared_ptr<Int> x4=(*x2)*(*x2);
    Compare(x4->Serialize(),"int:4","Product");
    shared_ptr<Int> x3=(*x4)-(*x1);
    Compare(x3->Serialize(),"int:3","Difference");
    shared_ptr<Int> y2=(*x4)/(*x2);
    Compare(y2->Serialize(),"int:2","Quotient");
    Compare(((*x1)<=(*x2))->GetValue(),true,"LEQ");
    Compare(((*x3)<=(*x2))->GetValue(),false,"LEQ");
    Compare(((*x2)==(*y2))->GetValue(),true,"EQ");
    Compare(((*x3)==(*y2))->GetValue(),false,"EQ");
    Compare(((*y2)==(*x3))->GetValue(),false,"EQ");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
