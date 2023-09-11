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
        void Send(long int msg);
        void SingleSend(const std::shared_ptr<libpi::Value> &msg);
        void SingleSend(long int msg);
        void Send(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<libpi::Value> &msg);
        void Send(const std::shared_ptr<task::Task> &sender, long int msg);
        void SingleSend(const std::shared_ptr<task::Task> &sender, const std::shared_ptr<libpi::Value> &msg);
        void SingleSend(const std::shared_ptr<task::Task> &sender, long int msg);
        std::shared_ptr<libpi::Value> Receive();
        void Receive(long int &dest);
        bool Receive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<libpi::Value> &dest);
        bool Receive(const std::shared_ptr<task::Task> &receiver, long int &dest);
        std::shared_ptr<libpi::Value> SingleReceive();
        void SingleReceive(long int &dest);
        bool SingleReceive(const std::shared_ptr<task::Task> &receiver, std::shared_ptr<libpi::Value> &dest);
        bool SingleReceive(const std::shared_ptr<task::Task> &receiver, long int &dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        bool Empty() { return msgs.empty(); }

        // Thread channels are not serialized or parsed

      private:
        std::queue<std::shared_ptr<libpi::Value> > msgs;
        std::queue<long int> unsafemsgs;
        Mutex sync;
        Mutex unsafesync;
        Mutex lock;
        std::atomic<int> msg_count;
        std::atomic<int> unsafemsg_count;
    };
  }
}

#endif
