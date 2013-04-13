#ifndef libpi_channel_mq
#define libpi_channel_mq
// DOCUMENTATION: channel_pmq.hpp {{{
/*! \file
 * This file defines the interface for channels implemented with
 * SYSTEM V memmory queues.
 */
// }}}

#include <libpi/channel.hpp>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

namespace libpi
{
// DOCUMENTATION: channel_mq.hpp {{{
/*!
 * Implementation of the Channel interface based on the
 * SYSTEM V memory queue technology.
 */
// }}}
  class Channel_MQ : public Channel
  { public:
      // DOCUMENTATION Channel_MQ method {{{
      /*!
       * Create new channel from mqchannel address string
       */
      // }}}
      Channel_MQ(const std::string &address);
      // DOCUMENTATION Channel_MQ method {{{
      /*!
       * Create new channel from given queue key (or created new queue)
       */
      // }}}
      Channel_MQ(int key=-1);
      // DOCUMENTATION Channel_MQ method {{{
      /*!
       * Closes the channel, but does not unlink.
       */
      // }}}
      ~Channel_MQ();
      // DOCUMENTATION Unlink method {{{
      /*!
       * Unlink ensures that the underlying message queue will be
       * unlinked when the channel is closed (object is deleted).
       */
      // }}}
      void Unlink();

      // DOCUMENTATION Send method {{{
      /*!
       * Transmits the provided message using multiple messages,
       * first message contains the message size, and subsequent
       * messages the message data.
       * This requires linearity, but allows messages of arbitrary size.
       * @param msg contains the data to transmit.
       */
      // }}}
      void Send(Message &msg);
      // DOCUMENTATION Receive method {{{
      /*!
       * Receives data through multiple messages and stores it in the
       * provided message, first message contains the message size, and
       * subsequent messages the message data.
       * This requires linearity, but allows messages of arbitrary size.
       * @param msg destination of the received data.
       */
      // }}}
      void Receive(Message &msg);
      // DOCUMENTATION SingleSend method {{{
      /*!
       * Transmits the provided message using a single messages,
       * This eliminates the linearity requirement, but restricts the
       * message size.
       * @param msg contains the data to transmit.
       */
      // }}}
      void SingleSend(Message &msg);
      // DOCUMENTATION SingleReceive method {{{
      /*!
       * Receives data through a single message where the first 4 bytes
       * contain the message size, and all the data is in the remaining
       * buffer. The data is stored in the provided message.
       * This eliminates the linearity requirement, but restricts the
       * message size.
       * @param msg destination of the received data.
       */
      // }}}
      void SingleReceive(Message &msg);
      // DOCUMENTATION GetAddress method {{{
      /*!
       * Returns the queue name
       */
      // }}}
      std::string GetAddress() const;

    private:
      // DOCUMENTATION myKey field {{{
      /*!
       * Holds the queue kqy, used to establish a connection.
       */
      // }}}
      key_t myKey;
      // DOCUMENTATION myQueue field {{{
      /*!
       * Holds the queue identifier, representing this process'
       * connection to the queue.
       */
      // }}}
      int myQueue;
      // DOCUMENTATION myAttributes field {{{
      /*!
       * Holds the queue attributes such at message limit and max length.
       */
      // }}}
      msqid_ds myAttributes;
      // DOCUMENTATION myUnlink field {{{
      /*!
       * Stores if the queue shoulb be unlinked when closing channel.
       */
      // }}}
      bool myUnlink;
      // DOCUMENTATION ourQueueCounter field {{{
      /*!
       * A counter used to create unique queue names.
       */
      // }}}
      static unsigned int ourQueueCounter;
      static const int ourMessageLength;
  };
}
#endif
