#ifndef libpi_session_fifo
#define libpi_session_fifo
// DOCUMENTATION: session.hpp {{{
/*! \file
 * This file defines the libpi (pi-calculus operations for C++) thread level sessions
 * interface for implementation using an atomic Value vector.
 */
// }}}
#include <libpi/session.hpp>
#include <libpi/thread/channel.hpp>

namespace libpi
{ namespace thread
  { class Session : public libpi::Session
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
        Session(std::vector<libpi::Channel*> &chs, int pid, int actors);
        Session(std::vector<libpi::Channel*> &inChannels, std::vector<libpi::Channel> &outChannels, int pid, int actors);
  // DOCUMENTATION: Session destructor {{{
  /*!
  * The destructor closes the session (and all of its channels)
  * before deleting the object.
  * Reception channels are also unlinked (deleted).
  */
  // }}}
        virtual ~Session();
    
        void Send(int to, Value *value);
        Value *Receive(int from);
        void Delegate(int to, libpi::Session &s);
        void DelegateTo(libpi::Channel &to);
        Session *ReceiveSession(int from);
        void Close(bool unlink=true);
  
        static Session *creator_del(std::string address, int pid, int actors);
  
      private:
        std::vector<Channel*> myInChannels;
        std::vector<Channel*> myOutChannels;
    };
  }
}
#endif
