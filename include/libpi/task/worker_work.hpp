#pragma once
void libpi::task::Worker::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Find next task
      while (true)
      { if (myGCFlag) // Produce Mark results for Mark-Sweep
        { //cout << "WORKER(" << this << "): Marking" << endl;
          myGCFlag=false;
          myGCValues=myGCNewValues;
          myGCMarks=myGCNewMarks; 
          for (std::list<Task*>::iterator task=myActiveTasks.begin(); task!=myActiveTasks.end(); ++task)
            (*task)->Mark(myGCMarks);
          myGCReady=true; // GC results are ready
          myGCNewValues.clear();
          myGCNewMarks.clear();
          myGCDoneLock.Lock(); // Wait until it is safe to resume work
        }
        if (!myActiveTasks.empty())
          break;
        ourIdleWorkersLock.Lock();
        ourIdleWorkers.push_back(this);
        ++ourIdleWorkersSize;
        ourIdleWorkersLock.Release();
        //cout << "WORKER(" << this << "): Waiting for work" << endl;
        myWaitLock.Lock();
        //cout << "WORKER(" << this << "): Activated" << endl;
      }
      Task *task=myActiveTasks.front();
      myActiveTasks.pop_front();

      resume_task:
      if (!task)
        break;
      if (_methods(task))
      { if (myActiveTasks.empty() && !myGCFlag)
          goto resume_task;
        else QueueTask(task);
      }
      else
        --ActiveTasks;
    }
  } catch (const string &error)
  { cerr << "WORKER ERROR: " << error << endl;
  }
  return;
} // }}}
