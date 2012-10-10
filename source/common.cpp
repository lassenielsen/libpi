#include <libpi/common.hpp>
#include <unistd.h>
#include <vector>

using namespace libpi;
using namespace std;

TCPsocket connect(const IPaddress &channel) // {{{
{ int wait=     100000;
  int maxwait=10000000;
  IPaddress c=channel;
  TCPsocket socket=SDLNet_TCP_Open(&c);
  while (!socket) // retry connection until success
  { usleep(wait); // wait a bit before retrying
    wait = 2*wait;
    if (wait>maxwait)
      wait=maxwait;
    // retry
    socket = SDLNet_TCP_Open(&c);
  }
  return socket;
} // }}}

void send_msg(const TCPsocket &s, const void *msg, int msgSize) // {{{
{
} // }}}

void *receive_msg(int &size) // {{{
{ vector<char*> buffers;
  int size=1024;
  whil
+++++++e (size==1024)
  { char *buffer=new char[1024];
    size=SDLNet_TCP_Recv();
} // }}}
