#ifndef libpi_channel
#define libpi_channel
// DOCUMENTATION: channel.hpp {{{
/*! \file
 * This file defines the libpi (pi-calculus operations for C++) channel
 * structure and interface.
 */
// }}}
#include <string>
#include <map>
#include <libpi/value.hpp>

namespace libpi
{
// DOCUMENTATION: Channel class {{{
/*!
 * Channel is a communication point that is used to initiate sessions.
 * This can be a TCP port (used in Channel_TCP),
 * memory queue key (used in Channel_MQ)
 * or indeed any means of establishing communication.
 */
// }}}
  class Channel
  { public:
      typedef Channel *(*channel_creator)(std::string);
      virtual ~Channel() {}

// DOCUMENTATION: Create Method {{{
/*!
 * Copy creates a new copy of the channel.
 */
// }}}
      virtual Channel *Copy() const=0;
// DOCUMENTATION: Create Method {{{
/*!
 * Unlink marks the channel to be removed (and its ressources freed) when the
 * object is deleted.
 */
// }}}
      virtual void Unlink()=0;

      bool operator==(const Channel &rhs) {return GetAddress()==rhs.GetAddress(); }

// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a message on the channel.
 * The transmitted value is consumed.
 */
// }}}
      virtual void Send(Value *msg)=0;
// DOCUMENTATION: SingleSend method {{{
/*!
 * SingleSend transmits a message on the channel, ensuring the message
 * is not broken up in multiple packets. If message size exceedes the
 * packet limit, a string exception is thrown.
 * The transmitted value is consumed.
 */
// }}}
      virtual void SingleSend(Value *msg)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Receive returns the value received on the channel.
 */
// }}}
      virtual Value *Receive()=0;
// DOCUMENTATION: Receive method {{{
/*!
 * SingleReceive receives a single packet, and returns the contained
 * value.
 */
// }}}
      virtual Value *SingleReceive()=0;
// DOCUMENTATION: GetAddress accessor {{{
/*!
 * GetAddress is used to obtain a serialized address that can be used
 * to access and connect to the channel.
 * In the case of a TCP channel, the address will contain hostname and
 * port, in the case of memory queues, the address will be the queue
 * key.
 */
// }}}
      virtual std::string GetAddress() const=0;

// DOCUMENTATION: Create Method {{{
/*!
 * Create is a static method that provides a unified method of creating channels
 * of all the supported protocol types.
 */
// }}}
      static Channel *Create(const std::string &address);

      virtual Channel *Copy() const =0;

    private:
// DOCUMENTATION: ourSessionCreators field {{{
/*!
 * Maps all channel protocols to methods that can create a channel from an
 * (possibly empty) addres using the specified protocol.
 * This enables the static Create method which provides a unified method of
 * greating sessions.
 */
// }}}
      static std::map<std::string,channel_creator> ourChannelCreators;
  };
}
#endif
