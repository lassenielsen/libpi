#ifndef libpi_thread_channel
#define libpi_thread_channel

#include <pthread.h>
#include <libpi/mutex.hpp>
#include <libpi/channel.hpp>
#include <string>
#include <queue>

namespace libpi {
  namespace thread {
    class Channel : public libpi::Channel
    { public:
        Channel();
        Channel(const Channel &rhs);
        virtual ~Channel();

        void Send(Value *msg);
        void SingleSend(Value *msg);
        Value *Receive();
        Value *SingleReceive();
    
        std::string GetAddress() const;
    
        Channel *Copy() const;

        Channel &operator=(const Channel &rhs) // {{{
        { Detach();
          rhs.lock->Lock();
          ref_count=rhs.ref_count;
          ++(*ref_count);
          msgs=rhs.msgs;
          msg_count=rhs.msg_count;
          sync=rhs.sync;
          lock=rhs.lock;
          rhs.lock->Release();
          return *this;
        } // }}}

      protected:
        void Detach() // {{{
        { lock->Lock();
          --(*ref_count);
          if (*ref_count<=0)
          { delete ref_count;
            delete msg_count;
            sync->Release();
            delete sync;
            while (msgs->size()>0)
            { std::cout << "Deleting msg: " << msgs->front()->ToString() << std::endl;
              delete msgs->front();
              msgs->pop();
            }
            delete msgs;
            lock->Release();
            delete lock;
          }
          else
            lock->Release();

          ref_count=NULL;
          msg_count=NULL;
          lock=NULL;
          sync=NULL;
          msgs=NULL;
        } // }}}

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
