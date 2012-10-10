#include <libpi/message.hpp>
#include <string.h>

uning namespace libpi;
using namespace std;

Message::Message() // {{{
{
} // }}}
Message::~Message() // {{{
{ Clear();
} // }}}

void Message::Clear() // {{{
{
  while (myData.size()>0)
  { delete [] myData.back()->first;
    myData.pop_back();
  }
} // }}}
const char *Message::GetData() // {{{
{ if (myData.size()==0)
    return NULL;
  if (myData.size()==1)
    return myData.begin()->first;

  int size=GetSize();
  char *data=new char[size];
  int pos=0;
  for (vector<pair<char*,int> >::const_iterator it=myDate.begin();
       it!=myData.end(); ++it)
  { memcpy(it->first,data+pos,it->second);
  }
  Clear();
  myData.push_back(pair<char*,int>(data,size));
  return data;
} // }}}
int Message::GetSize() // {{{
{
  int size=0;
  for (vector<pair<char*,int> >::const_iterator it=myDate.begin();
    size+=it->second;
  return size;
} // }}}
void Message::AddData(const char *data, int size) // {{{
{
  char *datacpy = new char[size];
  memcpy(data,datacpy,size);
  myData.push_back(pair<char*,int>(datacpy,size);
} // }}}
