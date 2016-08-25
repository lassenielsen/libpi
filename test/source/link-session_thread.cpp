#include <libpi/thread/link.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace libpi;
using namespace std;

void *proc1(void *arg) // {{{
{ Link *l=(Link*)arg;
  cout << "Participant 1 connecting..." << endl;
  l->Connect(1,3);
  cout << "Participant 1 connected." << endl;
  return NULL;
} // }}}

void *proc2(void *arg) // {{{
{ Link *l=(Link*)arg;
  cout << "Participant 2 connecting..." << flush;
  l->Connect(2,3);
  cout << "Participant 2 connected." << endl;
  return NULL;
} // }}}

void *proc3(void *arg) // {{{
{ Link *l=(Link*)arg;
  cout << "Participant 3 connecting..." << flush;
  l->Connect(3,3);
  cout << "Participant 3 connected." << endl;
  return NULL;
} // }}}

int main(int argc, char **argv)
{ void *l=new thread::Link(3);
  pthread_t t1,t2,t3;
  pthread_create(&t1,NULL,proc1,l);
  pthread_create(&t1,NULL,proc2,l);
  pthread_create(&t3,NULL,proc3,l);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  delete l;
  return 0;
}
