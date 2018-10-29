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
#include <libpi/task/task.hpp>

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
      Session(int pid, int actors, std::vector<Channel*> &inChannels, std::vector<Channel*> &outChannels, gc::GCRegistrant *registrant);
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
 * @param value is the value to be sent, this is transfered to the
 * receiving participant.
 */
// }}}
      virtual void Send(int to, Value *value);
      virtual void Send(int to, task::Task *sender, Value *value);
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
      virtual Value *Receive(int from, gc::GCRegistrant *registrant);
      virtual bool Receive(int from, task::Task *receiver, Value *&dest);

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
      Bool *operator==(const Value &rhs) const;
      Bool *operator<=(const Value &rhs) const;
      Bool *operator<(const Value &rhs) const;
      Bool *operator>=(const Value &rhs) const;
      Bool *operator>(const Value &rhs) const;

      virtual void Mark(std::unordered_set<Value*> &marks);

      //static Value *ParseSession(const std::string &str);

    private:
      int myPid;
      int myActors;

      std::vector<Channel*> myInChannels;
      std::vector<Channel*> myOutChannels;
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
