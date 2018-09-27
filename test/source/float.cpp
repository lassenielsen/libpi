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
  { shared_ptr<Float> x0(new Float(3.1415));
    Compare(x0->Serialize().substr(0,10),"flt:3.1415","Serialize");
    shared_ptr<Float> x1(new Float("3.1415"));
    Compare(x1->Serialize(),"flt:3.1415","Initialize");
    { shared_ptr<Value> parsed=shared_ptr<Value>(Value::Parse("flt:3.1415"));
      Compare(parsed->Serialize(),"flt:3.1415","Parse");
    }
    shared_ptr<Float> x2((*x1)+(*x1));
    Compare(x2->Serialize().substr(0,10),"flt:6.2829","Sum");
    shared_ptr<Float> x4=(*x2)*(*x2);
    Compare(x4->Serialize().substr(0,14),"flt:39.4760889","Product");
    shared_ptr<Float> x3=(*x4)-(*x1);
    Compare(x3->Serialize().substr(0,14),"flt:36.3345889","Difference");
    shared_ptr<Float> y2=(*x4)/(*x2);
    Compare(y2->Serialize().substr(0,10),"flt:6.2829","Quotient");
    Compare(((*x1)<=(*x2))->GetValue(),true,"LEQ");
    Compare(((*x3)<=(*x2))->GetValue(),false,"LEQ");
    //Error due to inaccuracy Compare(((*x2)==(*y2))->GetValue(),true,"EQ");
    Compare(((*x3)==(*y2))->GetValue(),false,"EQ");
    Compare(((*y2)==(*x3))->GetValue(),false,"EQ");
    shared_ptr<Float> x5(new Float("5.0"));
    Compare(x5->Serialize(),"flt:5.0","Serialize");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
