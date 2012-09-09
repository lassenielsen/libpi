#include <libpi/session.hpp>
#include <libpi/common.hpp>

using namespace libpi;

Session::Session(const IPaddress &channel, int pid, int maxpid)
{ if (pid==0)
  { // receive requests from other participants and distribute channels
  }
  else
  { // connect to channel
    TCPsocket socket=connect(channel);
    // send own channels
    // receive channels for other participants
  }
}
