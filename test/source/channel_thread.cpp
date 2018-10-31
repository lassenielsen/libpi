#include "worker.hpp"
#include <libpi/thread/channel.hpp>
#include <libpi/session.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/string.hpp>
#include <libpi/tuple.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

using namespace libpi;
using namespace std;

//#define PIDEBUG

class procarg
{ public:
    procarg(Channel *c, Value *v, int w)
    : chan(c)
    , val(v)
    , wait(w)
    { }
    Channel *chan;
    Value *val;
    int wait;
};

void *proc_send(void *arg) // {{{
{ try
  { stringstream ss;
    procarg *parg=(procarg*)arg;
    if (parg->wait>0)
    {
#ifdef PIDEBUG
      stringstream ss;
      ss << "Sender waiting...\n";
      cout << ss.str() << flush;
#endif
      usleep(parg->wait);
    }
#ifdef PIDEBUG
    { stringstream ss;
      ss << "Sender sending value...\n";
      cout << ss.str() << flush;
    }
#endif
    parg->chan->Send(parg->val);
#ifdef PIDEBUG
    { stringstream ss;
      ss << "Sender done.\n";
      cout << ss.str() << flush;
    }
#endif
  }
  catch (string s)
  { cout << "Error in sender: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}

void *proc_receive(void *arg) // {{{
{ try
  { procarg *parg=(procarg*)arg;
    if (parg->wait>0)
    {
#ifdef PIDEBUG
      stringstream ss;
      ss << "Receiver waiting...\n";
      cout << ss.str() << flush;
#endif
      usleep(parg->wait);
    }
#ifdef PIDEBUG
    { stringstream ss;
      ss << "Receiving value...\n";
      cout << ss.str() << flush;
    }
#endif
    Value *rv=parg->chan->Receive(NULL);
#ifdef PIDEBUG
    { stringstream ss;
      ss << "Receiver done.\n";
      cout << ss.str() << flush;
    }
#endif
    if (rv!=parg->val)
      throw string("Received value ") + rv->ToString() + " differs from expected " + parg->val->ToString();
  }
  catch (string s)
  { cout << "Error in sender: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}

int main(int argc, char **argv)
{ try
  { pthread_t t1, t2;

    void *r1, *r2;
    cout << "- Testing transmission of Bool value\n" << flush;
    Channel *ch1(new thread::Channel(NULL));
    procarg argBool(ch1,Bool::GetInstance(true),0);
    pthread_create(&t1,NULL,proc_send,&argBool);
    pthread_create(&t2,NULL,proc_receive,&argBool);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of Int value\n" << flush;
    Channel *ch2(new thread::Channel(NULL));
    procarg argInt(ch2,new Int(1337,NULL),0);
    pthread_create(&t1,NULL,proc_send,&argInt);
    pthread_create(&t2,NULL,proc_receive,&argInt);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of String value\n" << flush;
    Channel *ch3(new thread::Channel(NULL));
    procarg argString(ch3,new String("Hello World",NULL),0);
    pthread_create(&t1,NULL,proc_send,&argString);
    pthread_create(&t2,NULL,proc_receive,&argString);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of Channel value\n" << flush;
    Value *channelValue(new thread::Channel(NULL));
    Channel *ch4(new thread::Channel(NULL));
    procarg argChannel(ch4,channelValue,0);
    pthread_create(&t1,NULL,proc_send,&argChannel);
    pthread_create(&t2,NULL,proc_receive,&argChannel);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of Session value\n" << flush;
    vector<Channel *> chVector;
    chVector.push_back(dynamic_cast<Channel*>(channelValue));
    Value *sessionValue(new Session(1,1,chVector,chVector,NULL));
    Channel *ch5(new thread::Channel(NULL));
    procarg argSession(ch5,sessionValue,0);
    pthread_create(&t1,NULL,proc_send,&argSession);
    pthread_create(&t2,NULL,proc_receive,&argSession);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of Tuple value\n" << flush;
    Tuple *tplVal=new Tuple(NULL);
    tplVal->AddValue(new String("Hello",NULL));
    tplVal->AddValue(new Int(123,NULL));
    tplVal->AddValue(new String("World",NULL));
    Channel *ch6(new thread::Channel(NULL));
    procarg argTuple(ch6,tplVal,0);
    pthread_create(&t1,NULL,proc_send,&argTuple);
    pthread_create(&t2,NULL,proc_receive,&argTuple);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing concurrency\n" << flush;
    Channel *ch7(new thread::Channel(NULL));
    for (size_t i=0; i<99; ++i)
    { pthread_t x;
      pthread_attr_t y;
      pthread_attr_init(&y);
      pthread_attr_setstacksize(&y,16384);
      pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
      pthread_create(&x,&y,proc_send,&argInt);
    }
    pthread_create(&t1,NULL,proc_send,&argInt);
    for (size_t i=0; i<99; ++i)
    { pthread_t x;
      pthread_attr_t y;
      pthread_attr_init(&y);
      pthread_attr_setstacksize(&y,16384);
      pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
      pthread_create(&x,&y,proc_receive,&argInt);
    }
    usleep(1000);
    pthread_create(&t2,NULL,proc_receive,&argInt);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
  }
  catch (string s)
  { cout << "Error in main: " << s << endl;
  }
  return 0;
}
