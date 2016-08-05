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
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        Channel *Copy() const;
        void Unlink();

        void Send(libpi::Value *msg);
        void SingleSend(libpi::Value *msg);
        libpi::Value *Receive();
        libpi::Value *SingleReceive();
    
        std::string GetAddress() const;

        Channel &operator=(const Channel &rhs);

      protected:
        void Detach();

      private:
        std::queue<libpi::Value*> *msgs;
        Mutex *sync;
        Mutex *lock;
        std::atomic<int> *msg_count;
        std::atomic<int> *ref_count;
    };
  }
}

#endif
