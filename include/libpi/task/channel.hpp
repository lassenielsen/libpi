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
 * libpi::task::Channel implements a task-level channel.
 * This allows tasks to send and receive messages in an efficient
 * way.
 * This is because the transmitted values are not serialized and
 * re-parsed as is necessary for process level channels and network
 * level channels.
 */
// }}}
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        void Unlink();

        // Unsupported interface
        void Send(libpi::Value *msg);
        void SingleSend(libpi::Value *msg);
        Value *Receive();
        Value *SingleReceive();

        // Actual methods
        void Send(Task *sender, libpi::Value *msg);
        void SingleSend(Task *sender, libpi::Value *msg);
        void Send(Task *sender, long int msg);
        void SingleSend(Task *sender, long int msg);
        bool Receive(Task *receiver, libpi::Value **dest);
        bool Receive(Task *receiver, long int *dest);
        bool SingleReceive(Task *receiver, libpi::Value **dest);
        bool SingleReceive(Task *receiver, long int *dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

        // Thread channels are not serialized or parsed

      private:
        pthread_mutex_t myLock;
        std::queue<std::pair<Task*,libpi::Value**> > myTasks;
        std::queue<libpi::Value*> myMsgs;
        std::queue<std::pair<Task*,long int*> > myIntTasks;
        std::queue<long int> myIntMsgs;
    };
  }
}

#endif
