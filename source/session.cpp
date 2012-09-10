#include <libpi/session.hpp>
#include <libpi/common.hpp>

using namespace libpi;
using namespace std;

Session::Session(const IPaddress &channel, int pid, int maxpid)
{ if (pid==0)
  { // receive requests from other participants and distribute channels
  }
  else
  { // connect to channel
    TCPsocket socket=connect(channel);
    vector<TCPSocket> inAddresses;
    vector<IPaddress> outAddresses;
    vector<IPaddress> cmdAddresses;
    for (int i=0; i<maxpid-1;++i)
    { IPaddress inAddr = new_channel();
      send_msg(socket, inAddr, sizeof(inAddr));
      inAddresses.push_back(inAddr);
    }
    for (int i=0; i<pid-1;++i)
    { IPaddress cmdAddr = new_channel();
      cmdAddresses.push_back(cmdAddr);
    
    // send own channels
    // receive channels for other participants
  }
}
