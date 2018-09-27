#include "worker.hpp"
#include <libpi/tuple.hpp>
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
  { vector<shared_ptr<Value> > vals;
    vals.push_back(shared_ptr<Value>(Value::Parse("boo:true")));
    vals.push_back(shared_ptr<Value>(Value::Parse("boo:false")));
    shared_ptr<Tuple> x1(new Tuple(vals));
    Compare(x1->Serialize(),"tpl:2:boo:true:boo:false","Serialize");
    { shared_ptr<Value> parsed=shared_ptr<Value>(Value::Parse("tpl:2:boo:true:boo:false"));
      Compare(parsed->Serialize(),"tpl:2:boo:true:boo:false","Parse");
    }
    Compare(x1->GetValue(0)->Serialize(),"boo:true","GetValue");
    Compare(x1->GetValue(1)->Serialize(),"boo:false","GetValue");
    x1->AddValue(shared_ptr<Value>(Value::Parse("int:123")));
    Compare(x1->Serialize(),"tpl:3:boo:true:boo:false:int:123","AddValue");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
