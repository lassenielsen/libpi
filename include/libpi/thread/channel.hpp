#ifndef libpi_thread_channel
#define libpi_thread_channel

#include <pthread.h>
#include <libpi/thread/mutex.hpp>
#include <libpi/channel.hpp>
#include <string>
#include <queue>
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

        void Send(Value *msg);
        void SingleSend(Value *msg);
        Value *Receive();
        Value *SingleReceive();
    
        std::string GetAddress() const;
    

        Channel &operator=(const Channel &rhs);

      protected:
        void Detach();

      private:
        std::queue<Value*> *msgs;
        Mutex *sync;
        Mutex *lock;
        int *msg_count;
        int *ref_count;
    };
  }
}

#endif
