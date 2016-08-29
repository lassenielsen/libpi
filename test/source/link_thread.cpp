#include <libpi/thread/link.hpp>
#include <libpi/int.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

using namespace libpi;
using namespace std;

class procarg
{ public:
    procarg(Link *l, int p, int a, int w)
    : link(l)
    , pid(p)
    , actors(a)
    , wait(w)
    { }
    Link *link;
    int pid;
    int actors;
    int wait;
};

void *proc(void *arg) // {{{
{ try
  { stringstream ss;
    procarg *parg=(procarg*)arg;
    if (parg->wait>0)
    { stringstream ss;
      ss << "Participant " << parg->pid+1 << " of " << parg->actors << " waiting...\n";
      cout << ss.str() << flush;
      usleep(parg->wait);
    }
    { stringstream ss;
      ss << "Participant " << parg->pid+1 << " of " << parg->actors << " connecting...\n";
      cout << ss.str() << flush;
    }
    shared_ptr<Session> s=parg->link->Connect(parg->pid,parg->actors);
    { stringstream ss;
      ss << "Participant " << parg->pid+1 << " of " << parg->actors << " connected.\n";
      cout << ss.str() << flush;
    }
    // Test session
    shared_ptr<Value> val;
    if (parg->pid==0)
    { val=shared_ptr<Value>(new Int(1));
      { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " sending.\n";
        cout << ss.str() << flush;
      }
      s->Send(1,val);
      { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " receiving.\n";
        cout << ss.str() << flush;
      }
      val=s->Receive(parg->actors-1);
      { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " finished.\n";
        cout << ss.str() << flush;
      }
    }
    else
    { { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " receiving.\n";
        cout << ss.str() << flush;
      }
      val=s->Receive(parg->pid-1);
      { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " sending.\n";
        cout << ss.str() << flush;
      }
      s->Send((parg->pid+1)%parg->actors,val);
      { stringstream ss;
        ss << "Participant " << parg->pid+1 << " of " << parg->actors << " finished.\n";
        cout << ss.str() << flush;
      }
    }
    if (val->ToString()!="1")
      throw string("Bad value received: ")+val->ToString();
  }
  catch (string s)
  { cout << "Error in proc: " << s << endl;
    return (void*)1;
  }
  return NULL;
} // }}}

int main(int argc, char **argv)
{ try
  { pthread_t t1,t2,t3;
    void *r1, *r2, *r3;
    cout << "- Testing 2 participants\n" << flush;
    thread::Link *l2=new thread::Link(2);
    procarg p1of2(l2,0,2,0);
    procarg p2of2(l2,1,2,0);
    cout << "--- Testing same time\n" << flush;
    pthread_create(&t1,NULL,proc,&p1of2);
    pthread_create(&t2,NULL,proc,&p2of2);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    cout << "--- Testing participant 1 delayed\n" << flush;
    p1of2.wait=1000000;
    pthread_create(&t1,NULL,proc,&p1of2);
    pthread_create(&t2,NULL,proc,&p2of2);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    cout << "--- Testing participant 2 delayed\n" << flush;
    p1of2.wait=0;
    p2of2.wait=1000000;
    pthread_create(&t1,NULL,proc,&p1of2);
    pthread_create(&t2,NULL,proc,&p2of2);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    if (r1==NULL && r2==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    delete l2;
    thread::Link *l3=new thread::Link(3);
    cout << "- Testing 3 participants\n" << flush;
    procarg p1of3(l3,0,3,0);
    procarg p2of3(l3,1,3,0);
    procarg p3of3(l3,2,3,0);
    cout << "--- Testing same time\n" << flush;
    pthread_create(&t1,NULL,proc,&p1of3);
    pthread_create(&t2,NULL,proc,&p2of3);
    pthread_create(&t3,NULL,proc,&p3of3);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    pthread_join(t3,&r3);
    if (r1==NULL && r2==NULL && r3==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    cout << "--- Testing participant 1 delayed\n" << flush;
    p1of3.wait=1000000;
    pthread_create(&t2,NULL,proc,&p1of3);
    pthread_create(&t3,NULL,proc,&p2of3);
    pthread_create(&t1,NULL,proc,&p3of3);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    pthread_join(t3,&r3);
    if (r1==NULL && r2==NULL && r3==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    cout << "--- Testing participant 2 delayed\n" << flush;
    p1of3.wait=0;
    p2of3.wait=1000000;
    pthread_create(&t2,NULL,proc,&p1of3);
    pthread_create(&t3,NULL,proc,&p2of3);
    pthread_create(&t1,NULL,proc,&p3of3);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    pthread_join(t3,&r3);
    if (r1==NULL && r2==NULL && r3==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    cout << "--- Testing participant 3 delayed\n" << flush;
    p2of3.wait=0;
    p3of3.wait=1000000;
    pthread_create(&t2,NULL,proc,&p1of3);
    pthread_create(&t3,NULL,proc,&p2of3);
    pthread_create(&t1,NULL,proc,&p3of3);
    pthread_join(t1,&r1);
    pthread_join(t2,&r2);
    pthread_join(t3,&r3);
    if (r1==NULL && r2==NULL && r3==NULL)
      cout << "SUCCESS\n" << flush;
    else
      cout << "FAILURE\n" << flush;
    delete l3;
  }
  catch (string s)
  { cout << "Error in main: " << s << endl;
  }
  return 0;
}
