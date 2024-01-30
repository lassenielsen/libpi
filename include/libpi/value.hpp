#pragma once
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <istream>
#include <iostream>
#include <atomic>
#include <queue>
#include <pthread.h>

namespace libpi
{

// DOCUMENTATION: Value class {{{
/*!
 * Value defines the common interface for all value, and also represents the unit values.
 * Defines interface for basic operations, and also holds the reference counting functionality.
 */
// }}}
class Value // {{{
{ public:
    typedef Value* (*value_creator)(std::istream &);
  public:
    Value();
    Value(const std::string &str);
    virtual ~Value();

    int RefCount() const { return myRefCount; }
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    std::string ToString() const;
    void Serialize(std::ostream &dest) const;
    std::string Serialize() const;
    virtual bool operator==(const Value &rhs) const;
    virtual bool operator<=(const Value &rhs) const;
    virtual bool operator<(const Value &rhs) const;
    virtual bool operator>=(const Value &rhs) const;
    virtual bool operator>(const Value &rhs) const;

    static Value* Parse(const std::string &str);
    static Value* Parse(std::istream &in);
    static int RegisterParser(const std::string &type, value_creator p);

    inline void AddRef() { ++myRefCount; }
    inline void UnRef() { --myRefCount; }
    inline int  RefCount() { return myRefCount; }

    inline static void Erase(Value *obj)
    { pthread_mutex_lock(&ourGarbageLock);
      delete obj; //ourGarbage.push(obj);
      pthread_mutex_unlock(&ourGarbageLock);
      //pthread_mutex_unlock(&ourGarbageWaitLock);
    }
    //inline static void GetGarbage(Value **obj)
    //{ pthread_mutex_lock(&ourGarbageLock);
    //  while (ourGarbage.empty())
    //  { pthread_mutex_unlock(&ourGarbageLock);
    //    pthread_mutex_lock(&ourGarbageWaitLock);
    //    pthread_mutex_lock(&ourGarbageLock);
    //  }
    //  *obj=ourGarbage.front();
    //  ourGarbage.pop();
    //  pthread_mutex_unlock(&ourGarbageLock);
    //}
    inline static int InitLocks()
    { pthread_mutex_init(&ourGarbageLock,NULL);
      //pthread_mutex_init(&ourGarbageWaitLock,NULL);
      return 1;
    }

  private:

    static std::map<std::string,value_creator> ourParsers;
    static std::queue<Value*> ourGarbage;
    static pthread_mutex_t ourGarbageLock;
    //static pthread_mutex_t ourGarbageWaitLock;
    std::atomic<int> myRefCount;
}; // }}}

inline void RemoveRef(Value *ref) // {{{
{ ref->UnRef();
  if (ref->RefCount()<=0)
  { // std::cout << "Deleting value at: " << (long int)this << std::endl;
    //delete ref;
    Value::Erase(ref);
  }
} // }}}
inline void AssignValue(Value **var, Value *val) // {{{
{ if (val==*var)
    return;
  val->AddRef();
  if (*var)
    RemoveRef(*var);
  *var=val;
} // }}}
inline void AssignNewValue(Value **var, Value *val) // {{{
{ if (*var)
    RemoveRef(*var);
  *var=val;
} // }}}
}
