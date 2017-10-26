#ifndef libpi_task_channel
#define libpi_task_channel

#include <pthread.h>
#include <libpi/thread/mutex.hpp>
#include <libpi/task/mutex.hpp>
#include <libpi/task/task.hpp>
#include <libpi/channel.hpp>
#include <string>
#include <queue>
#include <atomic>
#include <iostream>

namespace libpi {
  namespace task {
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

        // Unsupported interface
        std::shared_ptr<Value> Receive();
        std::shared_ptr<Value> SingleReceive();

        // Actual methods
        void Send(const std::shared_ptr<libpi::Value> &msg);
        void SingleSend(const std::shared_ptr<libpi::Value> &msg);
        bool Receive(const std::shared_ptr<Task> &receiver, std::shared_ptr<libpi::Value> &dest);
        bool SingleReceive(const std::shared_ptr<Task> &receiver, std::shared_ptr<libpi::Value> &dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        // Thread channels are not serialized or parsed

      private:
        pthread_mutex_t myLock;
        std::queue<std::pair<std::shared_ptr<Task>,std::shared_ptr<libpi::Value>&> > myTasks;
        std::queue<std::shared_ptr<libpi::Value> > myMsgs;
    };
  }
}

#endif
