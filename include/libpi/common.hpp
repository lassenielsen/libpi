#include "SDL_net.h"
#include <string>

namespace libpi
{
  TCPsocket connect(const IPaddress &channel);
  void send_msg(const TCPsocket &s, const void *msg, int msgSize);
  void *receive_msg(const TCPsocket &s, int &size);

}

int str2int(const std::string &s);
std::string int2str(int i);
