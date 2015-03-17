#include <libpi/message.hpp>
#include <string.h>
#include <stdlib.h>

using namespace libpi;
using namespace std;

Message::Message() // {{{
{
} // }}}
Message::Message(const string &str) // {{{
{ AddData(str.c_str(),str.size()+1);
} // }}}
Message::Message(const mpz_t &val) // {{{
{ 
  char *str=mpz_get_str(NULL,10,val);
  AddData(str,strlen(str)+1);
  free(str);
} // }}}
Message::~Message() // {{{
{ Clear();
} // }}}

void Message::Clear() // {{{
{
  while (myData.size()>0)
  { delete [] myData.back().first;
    myData.pop_back();
  }
} // }}}
const char *Message::GetData() // {{{
{ 
  if (myData.size()==0)
    return NULL;
  if (myData.size()==1)
    return myData.begin()->first;

  int size=GetSize();
  char *data=new char[size];
  int pos=0;
  for (vector<pair<char*,int> >::const_iterator it=myData.begin();
       it!=myData.end(); ++it)
  { memcpy(data+pos,it->first,it->second);
    pos+=it->second;
  }
  Clear();
  myData.push_back(pair<char*,int>(data,size));
  return data;
} // }}}
int Message::GetSize() // {{{
{
  int size=0;
  for (vector<pair<char*,int> >::const_iterator it=myData.begin(); it!=myData.end(); ++it)
    size+=it->second;
  return size;
} // }}}
void Message::AddData(const char *data, int size) // {{{
{
  char *datacpy = new char[size];
  memcpy(datacpy,data,size);
  myData.push_back(pair<char*,int>(datacpy,size));
} // }}}
void Message::GetValue(IntValue &dest) // {{{
{ dest=IntValue(GetData());
} // }}}
void Message::GetValue(StringValue &dest) // {{{
{ dest=StringValue(GetData());
} // }}}
void Message::GetValue(BoolValue &dest) // {{{
{ dest=BoolValue(*this);
} // }}}
