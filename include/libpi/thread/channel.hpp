#ifndef libpi_thread_channel
#define libpi_thread_channel

#include <pthread.h>
#include <libpi/thread/mutex.hpp>
#include <libpi/channel.hpp>
#include <string>
#include <queue>
#include <atomic>
#include <iostream>

namespace libpi {
  namespace thread {
// DOCUMENTATION: Channel class {{{
/*!
 * libpi::thread::Channel implements a thread-level channel.
 * This allows threads to send and receive messages in an efficient
 * way.
 * This is because the transmitted values are not serialized and
 * re-parsed as is necessary for process level channels and network
 * level channels.
 * Thread level channels are implemented via a
 * std::queue<const Value*> structure,
 * synchronized using mutexes.
 */
// }}}
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        void Unlink();

        void Send(const libpi::Value *msg);
        void SingleSend(const libpi::Value *msg);
        void Send(const task::Task *sender, const libpi::Value *msg);
        void SingleSend(const task::Task *sender, const libpi::Value *msg);
        const libpi::Value *Receive();
        bool Receive(const task::Task *receiver, const libpi::Value *&dest);
        const libpi::Value *SingleReceive();
        bool SingleReceive(const task::Task *receiver, const libpi::Value *&dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        bool Empty() { return msgs.empty(); }

        // Thread channels are not serialized or parsed

      private:
        std::queue<const libpi::Value*> msgs;
        Mutex sync;
        Mutex lock;
        std::atomic<int> msg_count;
    };
  }
}

#endif
