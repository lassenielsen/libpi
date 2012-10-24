#ifndef libpi_channel
#define libpi_channel
// DOCUMENTATION: channel.hpp {{{
/*! \file
 * This file defines the cpi (pi-calculus operations for C++) channel
 * structure and interface.
 */
// }}}
#include <string>
#include <map>
#include <libpi/message.hpp>

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

// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a message on the channel.
 */
// }}}
      virtual void Send(Message &msg)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Receive waits for a message on the channel, receives and stores the message
 * in the argument reference.
 */
// }}}
      virtual void Receive(Message &msg)=0;
// DOCUMENTATION: GetAddress accessor {{{
/*!
 * GetAddress is used to obtain a serialized address that can be used
 * to access and connect to the channel.
 * In the case of a TCP channel, the address will contain hostname and
 * port, in the case of memory queues, the address will be the queue
 * key.
 */
// }}}
      virtual std::string GetAddress()=0;

// DOCUMENTATION: Create Method {{{
/*!
 * Create is a static method that provides a unified method of creating channels
 * of all the supported protocol types.
 */
// }}}
      static Channel *Create(const std::string &address);

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
