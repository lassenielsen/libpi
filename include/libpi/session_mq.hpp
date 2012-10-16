#ifndef libpi_session_mq
#define libpi_session_mq
// DOCUMENTATION: session_mq.hpp {{{
/*! \file
 * This file denines the cpi (pi-calculus operations for C++) session
 * interface for implementation using memmory queues.
 */
// }}}
#include <libpi/session.hpp>

namespace libpi
{
  class Session_MQ
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
  
    private:
      virtual void DelegateTo(Channel &to);
  };
}
#endif

