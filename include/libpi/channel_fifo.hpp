#ifndef libpi_channel_fifo
#define libpi_channel_fifo
// DOCUMENTATION: channel_pfifo.hpp {{{
/*! \file
 * This file defines the interface for channels implemented with
 * SYSTEM V memmory queues.
 */
// }}}

#include <libpi/channel.hpp>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fstream>

namespace libpi
{
// DOCUMENTATION: channel_fifo.hpp {{{
/*!
 * Implementation of the Channel interface based on the
 * SYSTEM V memory queue technology.
 */
// }}}
  class Channel_FIFO : public Channel
  { public:
      // DOCUMENTATION Channel_FIFO method {{{
      /*!
       * Create new channel from (fifo) filename string
       */
      // }}}
      Channel_FIFO(const std::string &path="");
      // DOCUMENTATION Channel_FIFO method {{{
      /*!
       * Closes the channel, but does not unlink.
       */
      // }}}
      ~Channel_FIFO();
      // DOCUMENTATION Unlink method {{{
      /*!
       * Unlink ensures that the underlying fifo file will be
       * deleted when the channel is closed (object is deleted).
       */
      // }}}
      void Unlink();

      // DOCUMENTATION Send method {{{
      /*!
       * Writes the provided message in the underlying fifo file.
,
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
      // DOCUMENTATION myPath field {{{
      /*!
       * Holds the path to the fifo file used for the channel
       * communication.
       */
      // }}}
      std::string myPath;
      // DOCUMENTATION myStream field {{{
      /*!
       * Holds the file stream used to write to and read from the fifo
       * file.
       */
      // }}}
      std::fstream myStream;
      bool myUnlink;
      static const int ourMessageLength;
  };
}
#endif
