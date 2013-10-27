#ifndef libpi_session_fifo
#define libpi_session_fifo
// DOCUMENTATION: session_fifo.hpp {{{
/*! \file
 * This file defines the libpi (pi-calculus operations for C++) session
 * interface for implementation using fifo files.
 */
// }}}
#include <libpi/session.hpp>
#include <libpi/channel_fifo.hpp>

namespace libpi
{
  class Session_FIFO : public Session
  { public:
// DOCUMENTATION: Session constructor {{{
/*!
* The constructor initiates a session over the given connection.
* The connection requires @actors processes to create a session.
* The @pid's 0 through @actors-1 are used, where pid=0 waits for
* processes with pid 1 to @actors -1 to connect on the channel to
* create a joint session.
* @param actors the number of required processes in the created
* session.
*/
// }}}
      Session_FIFO(std::vector<Channel*> &chs, int pid, int actors);
// DOCUMENTATION: Session destructor {{{
/*!
* The destructor closes the session (and all of its channels)
* before deleting the object.
* Reception channels are also unlinked (deleted).
*/
// }}}
      virtual ~Session_FIFO();
  
      void Send(int to, Message &msg);
      void Receive(int from, Message &msg);
      void Delegate(int to, Session &s);
      void DelegateTo(Channel &to);
      Session *ReceiveSession(int from);
      void Close(bool unlink=true);

      static Session *creator_del(std::string address, int pid, int actors);

    private:
      Session_FIFO(std::vector<Channel_FIFO*> &inChannels, std::vector<Channel_FIFO*> &outChannels, int pid, int actors);

      std::vector<Channel_FIFO*> myInChannels;
      std::vector<Channel_FIFO*> myOutChannels;
  };
}
#endif
