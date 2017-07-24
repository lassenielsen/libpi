#pragma once
#include <pthread.h>
#include <string>

namespace libpi {
  class Mutex
  { public:
      Mutex(bool locked=false)
      { pthread_mutex_init(&myMutex,NULL);
        if (locked)
          pthread_mutex_lock(&myMutex);
      }
      Mutex(const Mutex &init) {throw std::string("Copying a lock is forbidden"); }
      ~Mutex() { pthread_mutex_destroy(&myMutex); }

      Mutex &operator=(const Mutex &rhs) {throw std::string("Assigning a lock is forbidden"); }
      void Lock() {pthread_mutex_lock(&myMutex);}
      void Release() {pthread_mutex_unlock(&myMutex);}

    private:
      pthread_mutex_t myMutex;
  };
}
