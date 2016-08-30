#pragma once
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
#include <memory>
#include <libpi/channel.hpp>
#include <libpi/value.hpp>

namespace libpi
{
  class Session : public Value
  { public:
// DOCUMENTATION: Session constructor {{{
/*!
* The constructor initiates a session over the given connection.
* The connection requires @actors processes to create a session.
* The @pid's 0 through @actors-1 are used, where pid=0 waits for
* processes with pid 1 to @actors -1 to connect on the channel to
* create a joint session.
* @param actors in the session. This must correspond to the number of
* channels in each of the provided vectors.
* @param inChannels the channel objects used to receive values from
* participants in the session.
* @param outChannels the channel objects used to send values to
* participants in the session.
*/
// }}}
      Session(int pid, int actors, std::vector<std::shared_ptr<Channel> > &inChannels, std::vector<std::shared_ptr<Channel> > &outChannels);
// DOCUMENTATION: Session destructor {{{
/*!
* The destructor closes the session (and all of its channels)
* before deleting the object.
*/
// }}}
      virtual ~Session();
  
// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a value on the channel that leads to the @to
 * participant of the session.
 * @param to is the index of the participant to receive the message.
 * @to should be less than the @actors of the session and not be the
 * pid of the sending process.
 * @param value is the value to be sent, this is transferedd to the
 * receiving participant and can no longer be used by the sender.
 */
// }}}
      virtual void Send(int to, std::shared_ptr<Value> value);
// DOCUMENTATION: Receive method {{{
/*!
 * Receive waits for and receives a value over the channel from the
 * @from participant of the session.
 * @param from is the number of the participant who sends the message.
 * @from should be less than the @actors of the session and not be the
 * pid of the receiving process.
 * @retuens a pointer to the received value.
 */
// }}}
      virtual std::shared_ptr<Value> Receive(int from);
//      virtual std::string Sync(std::vector<std::string> choices)=0;

// DOCUMENTATION: Close method {{{
/*!
 * Close closes the session, disabeling further communication.
 */
// }}}
      virtual void Close(bool unlink=true);
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
      int GetActors() const;
// DOCUMENTATION: GetPid method {{{
/*!
 * GetPid returns process id of the current process in the session.
 */
// }}}
      int GetPid() const;

      // Value methods
      std::string GetType() const;
      std::string ToString() const;
      bool operator==(const Value &rhs) const;

      static Value *ParseSession(const std::string &str);

    private:
      int myPid;
      int myActors;

      std::vector<std::shared_ptr<Channel> > myInChannels;
      std::vector<std::shared_ptr<Channel> > myOutChannels;
// DOCUMENTATION: ourSessionCreators field {{{
/*!
 * Maps all session protocols to methods that can create a session from an
 * addres using the specified protocol.
 * This enables the static Create method which provides a unified method of
 * greating sessions.
 */
// }}}
      //static std::map<std::string,session_creator> ourSessionCreators;
  };
}
