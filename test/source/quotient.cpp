#include <libpi/quotient.hpp>
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
  { shared_ptr<Quotient> x0(new Quotient(3.1415));
    // Inaccuracy issue Compare(x0->Serialize(),"qrt:31415/10000","Serialize");
    shared_ptr<Quotient> x1(new Quotient("3.1415"));
    Compare(x1->Serialize(),"qrt:6283/2000","Initialize (float str)");
    shared_ptr<Quotient> z1(new Quotient("31415/10000"));
    Compare(z1->Serialize(),"qrt:6283/2000","Initialize (frac str)");
    { shared_ptr<Value> parsed=shared_ptr<Value>(Value::Parse("qrt:3.1415"));
      Compare(parsed->Serialize(),"qrt:6283/2000","Parse");
    }
    shared_ptr<Quotient> x2((*x1)+(*x1));
    Compare(x2->Serialize(),"qrt:6283/1000","Sum");
    shared_ptr<Quotient> x4=(*x2)*(*x2);
    Compare(x4->Serialize(),"qrt:39476089/1000000","Product");
    shared_ptr<Quotient> x3=(*x4)-(*x1);
    Compare(x3->Serialize(),"qrt:36334589/1000000","Difference");
    shared_ptr<Quotient> y2=(*x4)/(*x2);
    Compare(y2->Serialize(),"qrt:6283/1000","Quotient");
    Compare((*x1)<=(*x2),true,"LEQ");
    Compare((*x3)<=(*x2),false,"LEQ");
    Compare((*x2)==(*y2),true,"EQ");
    Compare((*x3)==(*y2),false,"EQ");
    Compare((*y2)==(*x3),false,"EQ");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
