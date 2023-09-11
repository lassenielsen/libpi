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
namespace libpi { namespace task { class Task; } }

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
    virtual void Send(const std::shared_ptr<Value> &msg)=0;
    virtual void Send(long int msg)=0;
// DOCUMENTATION: Send method {{{
/*!
 * Task level send method. Enables the receiving task to be sceduled on the
 * sending tasas worker, avoiding further synchronization issues.
 * Returns true if message was received directly, and false if the task was
 * added to queue, and the task skould be ended (untill it will automatically
 * bede requeued by the channel when the message is received.
 */
// }}}
    virtual void Send(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<Value> &msg)=0;
    virtual void Send(const std::shared_ptr<task::Task> &sender, long int msg)=0;
// DOCUMENTATION: SingleSend method {{{
/*!
 * SingleSend transmits a message on the channel, ensuring the message
 * is not broken up in multiple packets. If message size exceedes the
 * packet limit, a string exception is thrown.
 * The transmitted value is consumed.
 */
// }}}
    virtual void SingleSend(const std::shared_ptr<Value> &msg)=0;
    virtual void SingleSend(long int msg)=0;
// DOCUMENTATION: Send method {{{
/*!
 * Task level single packet send method.
 */
// }}}
    virtual void SingleSend(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<Value> &msg)=0;
    virtual void SingleSend(const std::shared_ptr<task::Task> &sender, long int msg)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Receive returns the value received on the channel.
 */
// }}}
    virtual std::shared_ptr<Value> Receive()=0;
    virtual void Receive(long int &dest)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Task level receive method. Enables the task to be stored in a local queue
 * until the desired message is received.
 * Returns true if message was received directly, and false if the task was
 * added to queue, and the task skould be ended (untill it will automatically
 * be requeued by the channel when the message is received.
 */
// }}}
    virtual bool Receive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<Value> &dest)=0;
    virtual bool Receive(const std::shared_ptr<task::Task> &receiver, long int &dest)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * SingleReceive receives a single packet, and returns the contained
 * value.
 */
// }}}
    virtual std::shared_ptr<Value> SingleReceive()=0;
    virtual void SingleReceive(long int &dest)=0;
// DOCUMENTATION: Receive method {{{
/*!
 * Task level receive method. Enables the task to be stored in a local queue until the decired message is received.
 * Returns true if message was received directly, and false if the task was
 * added to queue, and the task skould be ended (untill it will automatically
 * be requeued by the channel when the message is received.
 */
// }}}
    virtual bool SingleReceive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<Value> &dest)=0;
    virtual bool SingleReceive(const std::shared_ptr<task::Task> &receiver, long int &dest)=0;
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
