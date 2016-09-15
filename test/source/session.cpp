#include <libpi/thread/link.hpp>
#include <libpi/session.hpp>
#include <libpi/int.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

using namespace libpi;
using namespace std;

void *proc1(void *arg) // {{{
{ try
  { stringstream ss;
    thread::Link *link=(thread::Link*)arg;
    usleep(1000000);
    cout << "P1: Connecting\n" << flush;
    shared_ptr<Session> s=link->Connect(0,2);
    cout << "P1: Connecting complete\n" << flush;
    shared_ptr<Value> intval=shared_ptr<Value>(Value::Parse("int:1337"));
    cout << "P1: Sending simple value\n" << flush;
    s->Send(1,intval);
    cout << "P1: Creating local session\n" << flush;
    shared_ptr<Session> t1;
    shared_ptr<Session> t2;
    { vector<shared_ptr<Channel> > chsIn1;
      chsIn1.push_back(shared_ptr<Channel>(new thread::Channel()));
      chsIn1.push_back(shared_ptr<Channel>(new thread::Channel()));
      vector<shared_ptr<Channel> > chsOut1;
      chsOut1.push_back(chsIn1[0]);
      chsOut1.push_back(shared_ptr<Channel>(new thread::Channel()));
      t1=shared_ptr<Session>(new Session(0,2,chsIn1,chsOut1));
      vector<shared_ptr<Channel> > chsIn2;
      chsIn2.push_back(chsOut1[1]);
      chsIn2.push_back(shared_ptr<Channel>(new thread::Channel()));
      vector<shared_ptr<Channel> > chsOut2;
      chsOut2.push_back(chsIn1[1]);
      chsOut2.push_back(chsIn2[1]);
      t2=shared_ptr<Session>(new Session(1,2,chsIn2,chsOut2));
    }
    cout << "P1: Delegating local session\n" << flush;
    s->Send(1,t2);
    cout << "P1: Using delegated session\n" << flush;
    t1->Send(1,intval);
  }
  catch (string s)
  { cout << "Error in proc1: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}
void *proc2(void *arg) // {{{
{ try
  { stringstream ss;
    thread::Link *link=(thread::Link*)arg;
    usleep(1000000);
    cout << "P2: Connecting\n" << flush;
    shared_ptr<Session> s=link->Connect(1,2);
    cout << "P2: Connecting complete\n" << flush;
    shared_ptr<Int> intval1 = dynamic_pointer_cast<Int>(s->Receive(0));
    if (intval1->Serialize()!="int:1337")
      throw string("Received wrong value: ")+intval1->Serialize();
    cout << "P2: Recieved correct value\n" << flush;
    shared_ptr<Session> t= dynamic_pointer_cast<Session>(s->Receive(0));
    cout << "P2: Recieved session\n" << flush;
    cout << "P2: Using delegated session\n" << flush;
    shared_ptr<Int> intval2 = dynamic_pointer_cast<Int>(t->Receive(0));
    if (intval2->Serialize()!="int:1337")
      throw string("Received wrong value: ")+intval2->Serialize();
    cout << "P2: Received correct value\n" << flush;
  }
  catch (string s)
  { cout << "Error in proc1: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}

int main(int argc, char **argv)
{ try
  { pthread_t t1,t2,t3;
    void *r1, *r2, *r3;
    cout << "--- Creating link... " << flush;
    thread::Link *link=new thread::Link(2);
    cout << "done\n" << flush;
    cout << "--- Starting two participants... " << flush;
    pthread_create(&t1,NULL,proc1,link);
    pthread_create(&t2,NULL,proc2,link);
    cout << "done\n" << flush;
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    delete link;
  }
  catch (string s)
  { cout << "Error in main: " << s << endl;
  }
  return 0;
}
