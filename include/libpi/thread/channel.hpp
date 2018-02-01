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
 * std::queue<std::shared_ptr<const Value> > structure,
 * synchronized using mutexes.
 */
// }}}
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        void Unlink();

        void Send(const std::shared_ptr<libpi::Value> &msg);
        void SingleSend(const std::shared_ptr<libpi::Value> &msg);
        void Send(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<libpi::Value> &msg);
        void SingleSend(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<libpi::Value> &msg);
        std::shared_ptr<libpi::Value> Receive();
        bool Receive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<libpi::Value> &dest);
        std::shared_ptr<libpi::Value> SingleReceive();
        bool SingleReceive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<libpi::Value> &dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        bool Empty() { return msgs.empty(); }

        // Thread channels are not serialized or parsed

      private:
        std::queue<std::shared_ptr<libpi::Value> > msgs;
        Mutex sync;
        Mutex lock;
        std::atomic<int> msg_count;
    };
  }
}

#endif
