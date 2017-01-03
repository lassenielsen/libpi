#pragma once
// DOCUMENTATION: session.hpp {{{
/*! \file
 * This file denines the cpi (pi-calculus operations for C++) session
 * interface.
 */
// }}}
#include <string>
#include <vector>
#include <memory>
#include <libpi/channel.hpp>
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: Session class {{{
/*!
 * A Session is a collection of channels used to exchange messages between a
 * closed group of participants.
 * The session constructor requires the channels for exchanging messages to be
 * provided. One way to create the necessary channels and initiate a Session is
 * using the Link class and subclasses.
 * Once the channels are provided, the Session class provides the Send and
 * Receive operations.  These operations are only C++ typed, thus all the nasty
 * things that are avoided using various pi-calculus type-systems will occur
 * unless the programs are well written.
 */
// }}}
  class Session : public Value
  { public:
// DOCUMENTATION: Session constructor {{{
/*!
* The constructor initiates a session using the provided sessionchannels.
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
      void ToStream(std::ostream &dest) const;
      std::shared_ptr<Bool> operator==(const Value &rhs) const;
      std::shared_ptr<Bool> operator<=(const Value &rhs) const;
      std::shared_ptr<Bool> operator<(const Value &rhs) const;
      std::shared_ptr<Bool> operator>=(const Value &rhs) const;
      std::shared_ptr<Bool> operator>(const Value &rhs) const;

      static std::shared_ptr<Value> ParseSession(const std::string &str);

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
