#define PIDEBUG
#include <libpi/int.hpp>
#include <libpi/task/worker.hpp>
#include <libpi/gc/manager.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace libpi;
using namespace std;

class ValueTest : public libpi::Value // {{{
{ public:
    ValueTest(bool done, gc::GCRegistrant * registrant)
    : Value(registrant)
    , myDone(done)
    {}
    virtual ~ValueTest()
    { if (!myDone)
      { cerr << "Error: Deleting ValueTest " << this << " with undone status" << endl;
        throw string("ValueTest Error");
      }
      else
        cout << "Deleting ValueTest object at " << this << endl;
      ourDeletedValues.insert(this);
    }

    void Done() {myDone=true;}

    static std::set<libpi::Value*> ourDeletedValues;

  private:
    bool myDone;
}; // }}}
std::set<libpi::Value*> ValueTest::ourDeletedValues;
class TaskTest : public libpi::task::Task // {{{
{ public:
    TaskTest(libpi::task::Worker *worker,
             std::string method,
             libpi::Value* v1,
             libpi::Value *v2,
             libpi::Value *v3,
             libpi::Value *v4,
             libpi::Value *v5,
             libpi::Value *v6,
             libpi::Value *v7,
             libpi::Value *v8)
    : libpi::task::Task(worker)
    { SetWorker(worker);
      var_v1=v1;
      ref_v1=v1;
      var_v2=v2;
      ref_v2=v2;
      ref_v3=v3;
      ref_v4=v4;
      ref_v5=v5;
      ref_v6=v6;
      ref_v7=v7;
      ref_v8=v8;
    }

    libpi::Value *var_v1;
    libpi::Value *var_v2;
    libpi::Value *ref_v1;
    libpi::Value *ref_v2;
    libpi::Value *ref_v3;
    libpi::Value *ref_v4;
    libpi::Value *ref_v5;
    libpi::Value *ref_v6;
    libpi::Value *ref_v7;
    libpi::Value *ref_v8;
    std::string use_method;

    void Mark(std::unordered_set<Value*> &marks)
    {
      if (marks.count(this)>0)
        return;
      libpi::task::Task::Mark(marks);
      if (var_v1!=NULL) var_v1->Mark(marks);
      if (var_v2!=NULL) var_v2->Mark(marks);
    }
}; // }}}
// All Methods {{{
inline bool _methods(libpi::task::Task *&_task)
{ size_t _steps=0;
  void *_label=_task->GetLabel();
  if (_label!=NULL)
    goto *_label;
  method_Main:
  #define _this ((TaskTest*)_task)
  { // Main {{{
    _task->SetLabel(&&METHOD_MAIN_2);
    METHOD_MAIN_1:
    usleep(1000000);
    cout << "Ready for GC" << endl;
    return true;
    METHOD_MAIN_2:
    usleep(1000000);
    // Test all values but v1 and v2 has been cleaned!
    if (ValueTest::ourDeletedValues.count(_this->ref_v1)>0)
      cerr << "ERROR: v1 (" << _this->ref_v1 << ") was cleaned prematurely!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v2)>0)
      cerr << "ERROR: v2 (" << _this->ref_v2 << ") was cleaned prematurely!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v3)==0)
      cerr << "ERROR: v3 (" << _this->ref_v3 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v4)==0)
      cerr << "ERROR: v4 (" << _this->ref_v4 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v5)==0)
      cerr << "ERROR: v5 (" << _this->ref_v5 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v6)==0)
      cerr << "ERROR: v6 (" << _this->ref_v6 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v7)==0)
      cerr << "ERROR: v7 (" << _this->ref_v7 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v8)==0)
      cerr << "ERROR: v8 (" << _this->ref_v8 << ") was not cleaned!" << endl;
    ((ValueTest*)(_this->var_v1))->Done();
    _this->var_v1=NULL;
    ((ValueTest*)(_this->var_v2))->Done();
    _this->var_v2=NULL;
    _task->SetLabel(&&METHOD_MAIN_3);
    return true;
    METHOD_MAIN_3:
    usleep(1000000);
    // Test all values has been cleaned!
    if (ValueTest::ourDeletedValues.count(_this->ref_v1)==0)
      cerr << "ERROR: v1 (" << _this->ref_v1 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v2)==0)
      cerr << "ERROR: v2 (" << _this->ref_v2 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v3)==0)
      cerr << "ERROR: v3 (" << _this->ref_v3 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v4)==0)
      cerr << "ERROR: v4 (" << _this->ref_v4 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v5)==0)
      cerr << "ERROR: v5 (" << _this->ref_v5 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v6)==0)
      cerr << "ERROR: v6 (" << _this->ref_v6 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v7)==0)
      cerr << "ERROR: v7 (" << _this->ref_v7 << ") was not cleaned!" << endl;
    if (ValueTest::ourDeletedValues.count(_this->ref_v8)==0)
      cerr << "ERROR: v8 (" << _this->ref_v8 << ") was not cleaned!" << endl;
    delete _task;
    _task=NULL;
    return false;
  } // }}}
} // }}}
#include <libpi/task/worker_work.hpp>
void *_start_worker(void *arg) // {{{
{ libpi::task::Worker *worker=(libpi::task::Worker*)arg;
  worker->Work();
  delete worker;
  return NULL;
} // }}}

int main(int argc, char **argv)
{ try
  { vector<libpi::task::Worker*> workers;
    libpi::task::Worker *_worker=new libpi::task::Worker();
    workers.push_back(_worker);

    libpi::Value *v1=new ValueTest(false,_worker);
    libpi::Value *v2=new ValueTest(false,_worker);
    libpi::Value *v3=new ValueTest(true,_worker);
    libpi::Value *v4=new ValueTest(true,_worker);
    libpi::Value *v5=new ValueTest(true,_worker);
    libpi::Value *v6=new ValueTest(true,_worker);
    libpi::Value *v7=new ValueTest(true,_worker);
    libpi::Value *v8=new ValueTest(true,_worker);
    cout << "Expecting these addresses to be preserved:" << endl
         << "v1 at " << v1 << endl
         << "v2 at " << v2 << endl;
    cout << "Expecting these addresses to be cleaned:" << endl
         << "v3 at " << v3 << endl
         << "v4 at " << v4 << endl
         << "v5 at " << v5 << endl
         << "v6 at " << v6 << endl
         << "v7 at " << v7 << endl
         << "v8 at " << v8 << endl;
    { // Create main task
      libpi::task::Task *_main(new TaskTest(_worker,"",v1,v2,v3,v4,v5,v6,v7,v8));
      _main->SetLabel(NULL);
      _worker->AddTask(_main);
    }
    pthread_t x;
    pthread_attr_t y;
    pthread_attr_init(&y);
    pthread_attr_setstacksize(&y,16384);
    pthread_create(&x,&y,_start_worker,(void*)_worker);
    // Now do GC
    libpi::gc::GCManager::Manage(workers);
    pthread_join(x,NULL);
  }
  catch (string s)
  { cout << "FAILED: " << s << endl;
    return 1;
  }
  return 0;
}
