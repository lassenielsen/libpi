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
#include <libpi/channel.hpp>

namespace libpi
{
  class Session
  { public:
      typedef Session *(*session_creator)(std::string address, int pid, int actors);

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
      void Send(int to, const Message &msg);
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
      void Receive(int from, Message &msg);
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
      void Delegate(int to, Session &s);
// DOCUMENTATION: ReceiveSession method {{{
/*!
 * ReceiveSession "receives" a session on the channel from the
 * The session is received through a message containing a session address where
 * the received session can be created in the same way as creating a new
 * session.
 * @param from is the number of the participant who sends the message.
 * @from should be less than the @actors of the session and not be the
 * pid of the receiving process.
 */
// }}}
      Session *ReceiveSession(int from);
//      virtual std::string Sync(std::vector<std::string> choices)=0;

// DOCUMENTATION: Close method {{{
/*!
 * Close closes the session, disabeling further communication.
 */
// }}}
      void Close();
// DOCUMENTATION: Closed method {{{
/*!
 * Closed returns true if the session is closed.
 */
// }}}
      bool Closed();

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

// DOCUMENTATION: Create method {{{
/*!
 * Create is a static method that provides a unified method of creating sessions
 * of all the supported protocol types.
 * @param address has the form protocol://address@(pid,actors).
 * Protocol identifies the protocol of the session.
 * Address is the address of the channel where the session channels can be exchanged.
 * In practice this can be performed in different ways depending on the
 * underlying protocol, but they all include receiving the the session
 * information from the sender, and opening a connection to the session.
 * Pid and Actor are integers defining the number of actors in the protocol and
 * what process id the received session has.
 * session.
 */
// }}}
      static Session *Create(const std::string &address);

    protected:
      virtual void DelegateTo(Channel &to)=0;

      std::vector<Channel*> myInChannels;
      std::vector<Channel*> myOutChannels;

      int myPid;
      int myActors;

// DOCUMENTATION: ourSessionCreators field {{{
/*!
 * Maps all session protocols to methods that can create a session from an
 * addres using the specified protocol.
 * This enables the static Create method which provides a unified method of
 * greating sessions.
 */
// }}}
      static std::map<std::string,session_creator> ourSessionCreators;
  };
}
#endif
