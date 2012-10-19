#ifndef libpi_session_mq
#define libpi_session_mq
// DOCUMENTATION: session_mq.hpp {{{
/*! \file
 * This file denines the cpi (pi-calculus operations for C++) session
 * interface for implementation using memmory queues.
 */
// }}}
#include <libpi/session.hpp>
#include <libpi/channel_mq.hpp>

namespace libpi
{
  class Session_MQ : public Session
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
      Session_MQ(std::vector<Channel_MQ> &chs, int pid, int actors);
// DOCUMENTATION: Session destructor {{{
/*!
* The destructor closes the session (and all of its channels)
* before deleting the object.
*/
// }}}
      virtual ~Session_MQ();
  
      void Send(int to, Message &msg);
      void Receive(int from, Message &msg);
      void Delegate(int to, Session &s);
      void DelegateTo(Channel &to);
      Session *ReceiveSession(int from);
      void Close();
      bool Closed();

      static Session *creator_del(std::string address, int pid, int actors);

    private:
      Session_MQ(std::vector<Channel_MQ*> &inChannels, std::vector<Channel_MQ*> &outChannels, int pid, int actors);

      std::vector<Channel_MQ*> myInChannels;
      std::vector<Channel_MQ*> myOutChannels;
  };
}
#endif

