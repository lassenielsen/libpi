#pragma once
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
 * Channel is a communication medium that is used to initiate sessions.
 * This can be a TCP port (used in Channel_TCP),
 * memory queue key (used in Channel_MQ)
 * or indeed any means of establishing communication.
 */
// }}}
class Channel : public Value
{ public:
    typedef std::shared_ptr<Channel> (*channel_creator)(const std::string &);
    virtual ~Channel() {}

//* DOCUMENTATION: Unlink Method {{{
/*!
 * Unlink marks the channel to be removed (and its ressources freed) when the
 * object is deleted.
 */
// }}}
    virtual void Unlink()=0;

    std::shared_ptr<Bool> operator==(const Value &rhs) const;
    std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    std::shared_ptr<Bool> operator<(const Value &rhs) const;
    std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    std::shared_ptr<Bool> operator>(const Value &rhs) const;

// DOCUMENTATION: Send method {{{
/*!
 * Send transmits a message on the channel.
 * The transmitted value is consumed.
 */
// }}}
    virtual void Send(std::shared_ptr<Value> msg)=0;
// DOCUMENTATION: SingleSend method {{{
/*!
 * SingleSend transmits a message on the channel, ensuring the message
 * is not broken up in multiple packets. If message size exceedes the
 * packet limit, a string exception is thrown.
 * The transmitted value is consumed.
 */
// }}}
    virtual void SingleSend(std::shared_ptr<Value>msg)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Receive returns the value received on the channel.
 */
// }}}
    virtual std::shared_ptr<Value> Receive()=0;
// DOCUMENTATION: Receive method {{{
/*!
 * SingleReceive receives a single packet, and returns the contained
 * value.
 */
// }}}
    virtual std::shared_ptr<Value> SingleReceive()=0;
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
    static std::shared_ptr<Channel> Create(const std::string &address);

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
