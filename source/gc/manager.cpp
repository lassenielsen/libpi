#include <libpi/gc/manager.hpp>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <chrono>

using namespace libpi;
using namespace gc;
using namespace std;

auto t0=std::chrono::high_resolution_clock::now();
#define MNOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count()
void GCManager::Manage(vector<task::Worker*> &workers, size_t run_interval, size_t wait_interval, ManagerMode mode) // {{{
{ bool done=false;
  set<libpi::Value*> values;
  while (!done)
  { //cout << "GC(" << MNOW << "): Resting..." << endl;
    usleep(run_interval);
    //cout << "GC(" << MNOW << "): Running" << endl;
    // Test for termination
    if (workers.size()==libpi::task::Worker::GetIdleWorkersSize())
    { done=true;
      //cout << "GC(" << MNOW << "): Terminating when done" << endl;
    }
    //cout << "GC(" << MNOW << "): Set GC mode" << endl;
    set<libpi::task::Worker*> remaining(workers.begin(),workers.end());
    bool wait_remaining=false;
    while (!remaining.empty())
    { if (wait_remaining)
        usleep(wait_interval);
      else
        wait_remaining=true;
      //cout << "GC(" << MNOW << "): Set workers in GC mode" << endl;
      for (set<libpi::task::Worker*>::iterator w=remaining.begin(); w!=remaining.end(); ++w)
      { if ((*w)->GCReady())
        { w=remaining.erase(w);
        }
        else
        { (*w)->GCTask();
          libpi::task::Worker::GetIdleWorkersLock().Lock();
          list<libpi::task::Worker*>::iterator iw=std::find(libpi::task::Worker::GetIdleWorkers().begin(), libpi::task::Worker::GetIdleWorkers().end(),*w);
          if (iw!=libpi::task::Worker::GetIdleWorkers().end()) // Activate idle worker
          { //cout << "GC(" << MNOW << "): Activating worker at " << *w << endl;
            libpi::task::Worker::GetIdleWorkers().erase(iw);
            (*w)->Activate();
            --libpi::task::Worker::GetIdleWorkersSize();
          }
          libpi::task::Worker::GetIdleWorkersLock().Release();
        }
        if (w==remaining.end())
          break;
      }
    }
    unordered_set<libpi::Value*> marks;
    for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
    { cout << "GC(" << MNOW << "): Handling worker " << *w << endl;
      values.insert((*w)->GCValues().begin(),(*w)->GCValues().end());
      for (std::list<libpi::task::Task*>::iterator task=(*w)->GetActiveTasks().begin(); task!=(*w)->GetActiveTasks().end(); ++task)
      { cout << "GC(" << MNOW << "): Handling Task " << *task << endl;
        (*task)->Mark(marks);
      }
    }
    //for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
    //{ //cout << "GC(" << MNOW << "): Worker " << *w << " returned the values: ";
      //for (unordered_set<libpi::Value*>::const_iterator v=(*w)->GCValues().begin(); v!=(*w)->GCValues().end(); ++v)
      //  cout << *v << ",";
      //cout << endl;
    //  values.insert((*w)->GCValues().begin(),(*w)->GCValues().end());
      //cout << "GC(" << MNOW << "): Worker " << *w << " returned the marks: ";
      //for (unordered_set<libpi::Value*>::const_iterator v=(*w)->GCMarks().begin(); v!=(*w)->GCMarks().end(); ++v)
      //  cout << *v << ",";
      //cout << endl;
   //   marks.insert((*w)->GCMarks().begin(),(*w)->GCMarks().end());
   // }
    set<libpi::Value*> garbage;
    // garbage = value \ marks
    std::set_difference(values.begin(),values.end(),marks.begin(),marks.end(),std::inserter(garbage,garbage.end()));
    //cout << "GC(" << MNOW << "): Deleting unmarked values" << endl;
    for (set<libpi::Value*>::iterator val=garbage.begin(); val!=garbage.end(); ++val)
    { cout << "GC(" << MNOW << "): Deleting value at " << *val << endl;
      string t=(*val)->GetType();
      cout << "GC(" << MNOW << "): Deleting value " << t << ":" << flush;
      if (t!="ses" && t!="lnk")
        (*val)->ToStream(cout);
      cout << " at " << *val << endl;
      delete *val;
    }
    set<libpi::Value*> newValues;
    //cout << "GC(" << MNOW << "): Storing remaining values" << endl;
    // newValues = values \cap marks
    std::set_intersection(values.begin(),values.end(),marks.begin(),marks.end(),std::inserter(newValues,newValues.end()));
    values=newValues;
    for (vector<libpi::task::Worker*>::iterator w=workers.begin(); w!=workers.end(); ++w)
      (*w)->GCDone();
  }
  //cout << "GC(" << MNOW << "): Terminating tasks" << endl;
  size_t endedSize=0;
  while (endedSize<workers.size())
  { libpi::task::Worker::GetIdleWorkersLock().Lock();
    while (!libpi::task::Worker::GetIdleWorkers().empty())
    { libpi::task::Worker::GetIdleWorkers().front()->EmployTask(NULL);
      libpi::task::Worker::GetIdleWorkers().pop_front();
      --libpi::task::Worker::GetIdleWorkersSize();
      ++endedSize;
    }
    libpi::task::Worker::GetIdleWorkersLock().Release();
  }
} // }}}
