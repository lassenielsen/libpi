#include <libpi/channel.hpp>
#include <libpi/session.hpp>
#include <libpi/common.hpp>
#include <string>


using namespace libpi;
using namespace std;

Session::Session(const IPaddress &channel, int pid, int maxpid)
{ if (pid==0)
  { // receive requests from other participants and distribute channels
  }
  else
  { // connect to channel
    TCPsocket socket=connect(channel);
    vector<Channel> inAddresses;
    vector<Channel> outAddresses;
    vector<Channel> cmdAddresses;
    for (int i=0; i<maxpid-1;++i)
    { Channel inAddr;
      string address=inAddr.str();
      send_msg(socket, address.c_str(), address.size()+1);
      inAddresses.push_back(inAddr);
    }
    for (int i=0; i<pid-1;++i)
    { Channel cmdAddr;
      cmdAddresses.push_back(cmdAddr);
    }
    
    // send own channels
    // receive channels for other participants
  }
}
