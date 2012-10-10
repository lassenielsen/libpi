#ifndef libpi_channel
#define libpi_channel
// DOCUMENTATION: channel.hpp {{{
/*! \file
 * This file defines the cpi (pi-calculus operations for C++) channel
 * structure and interface.
 */
// }}}
#include <libpi/session.hpp>
#include <string>

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
      Channel()=0;
      virtual ~Channel()=0;

// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a message on the channel.
 */
// }}}
      virtual void Send(const Message &msg)=0;
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
  };
}
#endif
