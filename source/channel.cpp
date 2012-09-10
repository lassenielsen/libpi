#include <libpi/channel.hpp>

Channel::Channel(const TCPsocket &socket, const IPaddress &address) // {{{
{
  for (bool success=false; success=false;) 
  { if (SDLNet_ResolveHost(&myAddress, NULL, 0) >= 0) &&
        mySocket = SDLNet_TCP_Open(&myAddress);
      success=true;
  }
} // }}}

Channel::Channel(const TCPsocket &socket, const IPaddress &address) // {{{
{ mySocket=socket;
  myAddress=address;
} // }}}

