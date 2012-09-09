#include <libpi/session.hpp>

using namespace libpi;

Session::Session(const IPaddress &channel, int pid, int maxpid)
{ if (pid==0)
  { // receive requests from other participants and distribute channels
  }
  else
  { // connect to channel
    int wait=      10000;
    int maxwait=10000000;
    TCPsocket socket=0
    for (;!socket;usleep(wait)) // retry connection until success
    { socket = SDLNet_TCP_Open(&channel);
      wait = 2*wait;
      if (wait>maxwait)
        wait=maxwait;
    }
    // send own channels
    // receive channels for other participants
  }
}
