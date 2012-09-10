#include <libpi/value.hpp>
#include <string.h>

using namespace libpi;

Value::Value(int size, const char *data) // {{{
: mySize(size)
{ if (mySize>0)
  { myData = new char[mySize];
    memcpy(myData,data,mySize);
  }
  else
    myData=NULL;
} // }}}

Value::~Value() // {{{
{ if (myData!=NULL)
  { delete myData;
    myData=NULL;
  }
} // }}}

int Value::GetSize() // {{{
{ return mySize();
} // }}}

const char *Value::GetData() // {{{
{ return myData;
} // }}}
