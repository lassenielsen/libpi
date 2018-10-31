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
  { vector<Value *> vals;
    vals.push_back(Value::Parse("boo:true",NULL));
    vals.push_back(Value::Parse("boo:false",NULL));
    Tuple x1(vals,NULL);
    Compare(x1.Serialize(),"tpl:2:boo:true:boo:false","Serialize");
    { Value *parsed=Value::Parse("tpl:2:boo:true:boo:false",NULL);
      Compare(parsed->Serialize(),"tpl:2:boo:true:boo:false","Parse");
      delete parsed;
    }
    Compare(x1.GetValue(0)->Serialize(),"boo:true","GetValue");
    Compare(x1.GetValue(1)->Serialize(),"boo:false","GetValue");
    x1.AddValue(Value::Parse("int:123",NULL));
    Compare(x1.Serialize(),"tpl:3:boo:true:boo:false:int:123","AddValue");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
  }
  return 0;
}
