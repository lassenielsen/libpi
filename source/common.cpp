#include <libpi/common.hpp>

using namespace libpi;

TCPsocket connect(const IPaddress &channel) // {{{
{ int wait=     100000;
  int maxwait=10000000;
  TCPsocket socket=SDLNet_TCP_Open(&channel);
  while (!socket) // retry connection until success
  { usleep(wait); // wait a bit before retrying
    wait = 2*wait;
    if (wait>maxwait)
      wait=maxwait;
    // retry
    socket = SDLNet_TCP_Open(&channel);
  }
  return socket;
} // }}}
