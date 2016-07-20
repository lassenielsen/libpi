#include <libpi/network/channel.hpp>
#include <unistd.h>

using namespace std;

namespace libpi
{ namespace network
  {

Channel::Channel() // {{{
{
  for (bool success=false; success=false;) 
  { if ((SDLNet_ResolveHost(&myAddress, NULL, 0) >= 0) &&
        (mySocket = SDLNet_TCP_Open(&myAddress)))
      success=true;
  }
} // }}}

Channel::Channel(const TCPsocket &socket, const IPaddress &address) // {{{
{ mySocket=socket;
  myAddress=address;
} // }}}

TCPsocket Channel::Accept() // {{{
{ TCPsocket s;
  while (!(s = SDLNet_TCP_Accept(mySocket)))
    usleep(100000); // No Connection: Wait a bit
  return s;
} // }}}

IPaddress Channel::GetAddress() // {{{
{
  return myAddress;
} // }}}

string Channel::str() const // {{{
{ string result;
  // TODO: Create result
  return result;
} // }}}

  }
}
