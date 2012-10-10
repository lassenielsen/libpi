#include "SDL_net.h"

namespace libpi
{
  TCPsocket connect(const IPaddress &channel);
  void send_msg(const TCPsocket &s, const void *msg, int msgSize);
  void *receive_msg(const TCPsocket &s, int &size);
}
