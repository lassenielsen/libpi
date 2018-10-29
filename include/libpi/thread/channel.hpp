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
        Channel(libpi::gc::GCRegistrant *registrant);
        Channel(const Channel &rhs/* No registrant, as it is used to intercept std copy constructor! , libpi::gc::GCRegistrant *registrant*/);
        virtual ~Channel();

        void Unlink();

        void Send(libpi::Value *msg);
        void SingleSend(libpi::Value *msg);
        void Send(const task::Task *sender, libpi::Value *msg);
        void SingleSend(const task::Task *sender, libpi::Value *msg);
        libpi::Value *Receive(libpi::gc::GCRegistrant *registrant);
        bool Receive(task::Task *receiver, libpi::Value *&dest);
        libpi::Value *SingleReceive(libpi::gc::GCRegistrant *registrant);
        bool SingleReceive(task::Task *receiver, libpi::Value *&dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        bool Empty() { return msgs.empty(); }

        // Thread channels are not serialized or parsed

      private:
        std::queue<libpi::Value*> msgs;
        Mutex sync;
        Mutex lock;
        std::atomic<int> msg_count;
    };
  }
}

#endif
