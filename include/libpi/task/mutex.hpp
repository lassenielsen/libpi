#pragma once
#include <pthread.h>
#include <string>
#include <queue>
#include <libpi/task/task.hpp>

namespace libpi {
  namespace task {
    class Mutex
    { public:
        Mutex(bool locked=false)
        { pthread_mutex_init(&myLock,NULL);
          isLocked=locked;
        }
        Mutex(const Mutex &init) {throw std::string("Copying a lock is forbidden"); }
        ~Mutex() { pthread_mutex_destroy(&myLock); }
  
        Mutex &operator=(const Mutex &rhs) {throw std::string("Assigning a lock is forbidden"); }
        void Lock(std::shared_ptr<Task> task)
        { bool wasLocked;
          pthread_mutex_lock(&myLock);
          wasLocked=isLocked;
          isLocked=true;
          if (wasLocked)
            myQueue.push(task);
          pthread_mutex_unlock(&myLock);
          if (wasLocked)
            throw TaskPauseEvent();
        }
        void Release()
        { std::shared_ptr<Task> nextTask(NULL);
          pthread_mutex_lock(&myLock);
          if (myQueue.size()>0)
          { nextTask=myQueue.front();
            myQueue.pop();
          }           
          else
            isLocked=false;
          pthread_mutex_unlock(&myLock);
          if (nextTask)
            throw TaskResumeEvent(nextTask);
        }
  
      private:
        pthread_mutex_t myLock;
        std::queue<std::shared_ptr<Task> > myQueue;
        bool isLocked;
    };
  }
}
