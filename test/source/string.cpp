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
  { { String hello("hello:world",NULL);
      Compare(hello.Serialize(),"str:aGVsbG86d29ybGQ=","Serialize");
    }
    { Value *parsed=Value::Parse("str:aGVsbG86d29ybGQ=",NULL);
      Compare(dynamic_cast<String*>(parsed)->GetValue(),"hello:world","Parse");
      delete parsed;
    }

    String hello("hello",NULL);
    String world("world",NULL);
    String helloworld(hello,world,String::OP_CONCAT,NULL);
    Compare(helloworld.GetValue(),"helloworld","Concatenate");
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
    return 1;
  }
  return 0;
}
