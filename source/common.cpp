#include <libpi/common.hpp>
#include <unistd.h>
#include <vector>
#include <sstream>

using namespace std;

namespace libpi
{

//TCPsocket connect(const IPaddress &channel) // {{{
//{ int wait=     100000;
//  int maxwait=10000000;
//  IPaddress c=channel;
//  TCPsocket socket=SDLNet_TCP_Open(&c);
//  while (!socket) // retry connection until success
//  { usleep(wait); // wait a bit before retrying
//    wait = 2*wait;
//    if (wait>maxwait)
//      wait=maxwait;
//    // retry
//    socket = SDLNet_TCP_Open(&c);
//  }
//  return socket;
//} // }}}
//
//void send_msg(const TCPsocket &s, const void *msg, int msgSize) // {{{
//{
//} // }}}
//
//void *receive_msg() // {{{
//{ vector<char*> buffers;
//  int size=1024;
//  while (size==1024)
//  { char *buffer=new char[1024];
//    //size=SDLNet_TCP_Recv();
//  }
//} // }}}

inline int str2int(const string &s) // {{{
{ stringstream ss;
  ss << s;
  int result;
  ss >> result;
  return result;
} // }}}

inline string int2str(int i) // {{{
{ stringstream ss;
  ss << i;
  return ss.str();
} // }}}

inline std::vector<std::string> stringsplit(const std::string &source, const std::string &delim) // {{{
{ std::vector<std::string> dest;
  if (delim=="")
    return dest;
  int lastpos=0;
  int pos = source.find(delim,lastpos);
  while (pos < source.size())
  {
    dest.push_back(source.substr(lastpos,pos-lastpos));
    lastpos=pos+delim.size();
    pos = source.find(delim,lastpos);
  }
  dest.push_back(source.substr(lastpos,source.size()-lastpos));
  return dest;
} // }}}

inline std::string stringreplace(const std::string &source, const std::string &from, const std::string &to) // {{{
{
  std::vector<std::string> split=stringsplit(source,from);
  std::stringstream dest;
  for (std::vector<std::string>::const_iterator it=split.begin();
       it!=split.end(); ++it)
  {
    if (it!=split.begin())
      dest << to;
    dest << *it;
  }
  return dest.str();
} // }}}

template <class T>
inline void DeleteVector(std::vector<T*> &container) // {{{
{
  while (container.size() > 0)
  {
    delete container.back();
    container.pop_back();
  }
} // }}}
}
