/** This file denines the cpi (pi-calculus operations for C++) interface
 ** These operations are only C++ typed, thus all the nasty things that are
 ** avoided using various pi-calculus type-systems will occur unless the
 ** programs are well written.
 ** Upon errors, exceptions will be thrown!
 */

#include <string>
#include <vector>
#include "SDL_net.h"
#include <libpi/value.hpp>

namespace libpi
{
  class Session
  { public:
      Session(const IPaddress &channel, int pid, int maxpid);
      ~Session();
  
      Send(int to, const Value &v);
      Value Receive(int from):
      Delegate(int, to Session &s);
      Session ReceiveSession();
      std::string Sync(std::vector<std::string> choices);
  
      void End();
      bool IsClosed() const;

    private:
      std::vector<TCPsocket> inChannels;
      std::vector<TCPsocket> outChannels;
      std::vector<TCPsocket> cmdChannels;
  };
}
