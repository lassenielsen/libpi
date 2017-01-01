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

class procarg
{ public:
    procarg(shared_ptr<Channel> c, shared_ptr<Value> v, int w)
    : chan(c)
    , val(v)
    , wait(w)
    { }
    shared_ptr<Channel> chan;
    shared_ptr<Value> val;
    int wait;
};

void *proc_send(void *arg) // {{{
{ try
  { stringstream ss;
    procarg *parg=(procarg*)arg;
    if (parg->wait>0)
    { stringstream ss;
      ss << "Sender waiting...\n";
      cout << ss.str() << flush;
      usleep(parg->wait);
    }
    { stringstream ss;
      ss << "Sender sending value...\n";
      cout << ss.str() << flush;
    }
    parg->chan->Send(parg->val);
    { stringstream ss;
      ss << "Sender done.\n";
      cout << ss.str() << flush;
    }
  }
  catch (string s)
  { cout << "Error in sender: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}

void *proc_receive(void *arg) // {{{
{ try
  { stringstream ss;
    procarg *parg=(procarg*)arg;
    if (parg->wait>0)
    { stringstream ss;
      ss << "Receiver waiting...\n";
      cout << ss.str() << flush;
      usleep(parg->wait);
    }
    { stringstream ss;
      ss << "Receiving value...\n";
      cout << ss.str() << flush;
    }
    shared_ptr<Value> rv=parg->chan->Receive();
    { stringstream ss;
      ss << "Receiver done.\n";
      cout << ss.str() << flush;
    }
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
    shared_ptr<Channel> ch1(new thread::Channel());
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
    shared_ptr<Channel> ch2(new thread::Channel());
    procarg argInt(ch2,shared_ptr<Value>(new Int(1337)),0);
    pthread_create(&t1,NULL,proc_send,&argInt);
    pthread_create(&t2,NULL,proc_receive,&argInt);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of String value\n" << flush;
    shared_ptr<Channel> ch3(new thread::Channel());
    procarg argString(ch3,shared_ptr<Value>(new String("Hello World")),0);
    pthread_create(&t1,NULL,proc_send,&argString);
    pthread_create(&t2,NULL,proc_receive,&argString);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;

    cout << "- Testing transmission of Channel value\n" << flush;
    shared_ptr<Value> channelValue(new thread::Channel());
    shared_ptr<Channel> ch4(new thread::Channel());
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
    vector<shared_ptr<Channel> > chVector;
    chVector.push_back(dynamic_pointer_cast<Channel>(channelValue));
    shared_ptr<Value> sessionValue(new Session(1,1,chVector,chVector));
    shared_ptr<Channel> ch5(new thread::Channel());
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
    shared_ptr<Tuple> tplVal=shared_ptr<Tuple>(new Tuple());
    tplVal->AddValue(shared_ptr<Value>(new String("Hello")));
    tplVal->AddValue(shared_ptr<Value>(new Int(123)));
    tplVal->AddValue(shared_ptr<Value>(new String("World")));
    shared_ptr<Channel> ch6(new thread::Channel());
    procarg argTuple(ch6,tplVal,0);
    pthread_create(&t1,NULL,proc_send,&argTuple);
    pthread_create(&t2,NULL,proc_receive,&argTuple);
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
