#include <libpi/thread/channel.hpp>
#include <libpi/session.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/string.hpp>
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

int main(int argc, char **argv)
{ try
  { pthread_t t1;
    shared_ptr<Value> boolValue(new Bool(true));
    shared_ptr<Value> intValue(new Int(1337));
    shared_ptr<Value> stringValue(new String("Hello World"));
    shared_ptr<Value> channelValue(new thread::Channel());
    vector<shared_ptr<Channel> > chVector;
    chVector.push_back(channelValue);
    shared_ptr<Value> sessionValue(new Session(1,1,chVector,chVector));

    void *r1;
    cout << "- Testing transmission of Bool value\n" << flush;
    shared_ptr<Channel> ch1=new thread::Channel();
    procarg arg(ch1,boolValue,0);
    pthread_create(&t1,NULL,proc_send,&arg);
    shared_ptr<Value> v=ch1->Receive();
    pthread_join(t1,&r1);
    if (r1==NULL && v->ToString()==boolValue->ToString())
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
  }
  catch (string s)
  { cout << "Error in main: " << s << endl;
  }
  return 0;
}
