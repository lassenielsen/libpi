#ifndef libpi_session
#define libpi_session
// DOCUMENTATION: session.hpp {{{
/*! \file
 * This file denines the cpi (pi-calculus operations for C++) session
 * interface.
 * These operations are only C++ typed, thus all the nasty things that
 * are avoided using various pi-calculus type-systems will occur unless
 * the programs are well written.
 * Upon errors, exceptions will be thrown!
 */
// }}}
#include <string>
#include <vector>
#include <libpi/value.hpp>

namespace libpi
{
  class Session
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
      Session(int pid, int actors);
// DOCUMENTATION: Session destructor {{{
/*!
* The destructor closes the session (and all of its channels)
* before deleting the object.
*/
// }}}
      virtual ~Session();
  
// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a message on the channel that leads to the @to
 * participant of the session.
 * @param to is the number of the participant to receive the message.
 * @to should be less than the @actors of the session and not be the
 * pid of the sending process.
 * @param msg is the message to send.
 */
// }}}
      virtual void Send(int to, const Message &msg)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Receive waits for and receives a message on the channel from the
 * @from participant of the session.
 * @param from is the number of the participant who sends the message.
 * @from should be less than the @actors of the session and not be the
 * pid of the receiving process.
 * @param msg is the message to send.
 */
// }}}
      virtual void Receive(int from, Message &msg)=0;
// DOCUMENTATION: Delegate method {{{
/*!
 * Delegate "transmits" a session to the @to participant of the session.
 * In practice this can be performed in different ways depending on the
 * underlying technology, but they all include sending the session
 * information to the receiver, and closing the local copy of the
 * session.
 * @param to is the number of the participant to receive the message.
 * @to should be less than the @actors of the session and not be the
 * pid of the sending process.
 * @s the session to transmit.
 */
// }}}
      virtual void Delegate(int to, Session &s)=0;
// DOCUMENTATION: ReceiveSession method {{{
/*!
 * ReceiveSession "receives" a session on the channel from the
 * In practice this can be performed in different ways depending on the
 * underlying technology, but they all include receiving the the session
 * information from the sender, and opening a connection to the session
 * session.
 * @from participant of the session.
 * @param from is the number of the participant who sends the message.
 * @from should be less than the @actors of the session and not be the
 * pid of the receiving process.
 * @param msg is the message to send.
 */
// }}}
      virtual Session ReceiveSession()=0;
//      virtual std::string Sync(std::vector<std::string> choices)=0;

// DOCUMENTATION: Close method {{{
/*!
 * Close closes the session, disabeling further communication.
 */
// }}}
      virtual void Close()=0;
// DOCUMENTATION: Closed method {{{
/*!
 * Closed returns true if the session is closed.
 */
// }}}
      virtual bool Closed()=0;

// DOCUMENTATION: GetActors method {{{
/*!
 * GetActors returns the number of actors/participants in the session.
 */
// }}}
      int GetActors();
// DOCUMENTATION: GetPid method {{{
/*!
 * GetPid returns process id of the current process in the session.
 */
// }}}
      int GetPid();

    private:
      int myPid;
      int myActors;
  };
}
#endif
