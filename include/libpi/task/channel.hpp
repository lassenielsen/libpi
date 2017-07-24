#pragma once
#include <libpi/channel.hpp>
#include <libpi/thread/mutex.hpp>
#include <libpi/task/task.hpp>
#include <queue>
#include <atomic>

namespace libpi {
  namespace task {
// DOCUMENTATION: Channel class {{{
/*!
 * libpi::task::Channel implements a task-level channel.
 * This allows tasks to send and receive messages in an efficient
 * way.
 * This is because
 *  * The transmitted values are not serialized and re-parsed as is necessary
 *    for process level channels and network level channels.
 *  * The waiting is not mutex based, eliminating the need for a waiting thread
 * Task level channels are implemented via a
 * std::queue<std::shared_ptr<Task> > structure,
 * synchronized using mutexes.
 */
// }}}
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        void Unlink();

        void Send(std::shared_ptr<libpi::Value> msg);
        void SingleSend(std::shared_ptr<libpi::Value> msg);
        std::shared_ptr<Value> Receive();
        std::shared_ptr<libpi::Value> Receive(std::shared_ptr<Task> receiver, std::string dest);
        std::shared_ptr<Value> SingleReceive();
        std::shared_ptr<libpi::Value> SingleReceive(std::shared_ptr<Task> receiver, std::string dest);
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

      private:
        std::queue<std::shared_ptr<libpi::Value> > msgs;
        std::queue<std::pair<std::shared_ptr<Task>,std::string> > tasks;
        Mutex lock;
    };
  }
}
