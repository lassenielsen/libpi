#ifndef libpi_channel
#define libpi_channel
/** This file defines the cpi (pi-calculus operations for C++) channel structure and interface.
 */
#include <SDL_net.h>

namespace libpi
{
  class Channel
  { public:
      Channel(); // Create new channel
      Channel(const TCPsocket &socket, const IPaddress &address);
      ~Channel();

      TCPsocket Accept();
      IPaddress GetAddress();

    private:
      TCPsocket mySocket;
      IPaddress myAddress;
  };
}
#endif
