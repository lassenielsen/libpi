#include "worker.hpp"
#include <libpi/string.hpp>
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

int main(int argc, char **argv)
{ try
  { { shared_ptr<String> hello(new String("hello:world"));
      Compare(hello->Serialize(),"str:aGVsbG86d29ybGQ=","Serialize");
    }
    { shared_ptr<Value> parsed=shared_ptr<Value>(Value::Parse("str:aGVsbG86d29ybGQ="));
      Compare(dynamic_pointer_cast<String>(parsed)->GetValue(),"hello:world","Parse");
    }

    shared_ptr<String> hello=shared_ptr<String>(new String("hello"));
    shared_ptr<String> world=shared_ptr<String>(new String("world"));
    Compare(((*hello)+(*world))->GetValue(),"helloworld","Add");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
    return 1;
  }
  return 0;
}
