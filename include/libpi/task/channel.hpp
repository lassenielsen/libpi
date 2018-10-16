#ifndef libpi_task_channel
#define libpi_task_channel

#include <pthread.h>
#include <libpi/thread/mutex.hpp>
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
 * std::queue<Value*> structure,
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
        void Send(const libpi::Value *msg);
        void SingleSend(const libpi::Value *msg);
        Value *Receive();
        Value *SingleReceive();

        // Actual methods
        void Send(const Task *sender, const libpi::Value *msg);
        void SingleSend(const Task *sender, const libpi::Value *msg);
        bool Receive(const Task *receiver, libpi::Value *dest);
        bool SingleReceive(const Task *receiver, libpi::Value *dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        void Mark(std::unordered_set<void *> &marks) const; // Mark for garbage collection
        // Thread channels are not serialized or parsed

      private:
        pthread_mutex_t myLock;
        std::queue<std::pair<Task*,libpi::Value*> > myTasks;
        std::queue<libpi::Value*> myMsgs;
    };
  }
}

#endif
