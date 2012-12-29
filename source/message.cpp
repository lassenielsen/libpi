#include <libpi/message.hpp>
#include <string.h>

using namespace libpi;
using namespace std;

Message::Message() // {{{
{
} // }}}
Message::Message(const string &str) // {{{
{ AddData(str.c_str(),str.size()+1);
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
Message::operator string () // {{{
{ return (string)GetData();
}
