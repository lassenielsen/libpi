#include <libpi/task/task.hpp>

using namespace std;
using namespace libpi::task;

size_t Task::MaxSteps=100; // Maximum number of steps before yielding

Task::~Task() // {{{
{
} // }}}

void Task::ToStream(std::ostream &dest) const // {{{
{ throw std::string("Task::ToStream not implemented");
} // }}}

std::string Task::GetType() const // {{{
{ return "sta";
} // }}}

void Task::Mark(unordered_set<Value*> &marks) // {{{
{ if (marks.count(this)>0)
    return;
  
  marks.insert(this);
  if (tmp!=NULL)
    tmp->Mark(marks);
  for (vector<libpi::Value*>::const_iterator it=tmps.begin(); it!=tmps.end(); ++it)
    (*it)->Mark(marks);
  return;
} // }}}

std::string Task::GetName() const // {{{
{ return "Unknown";
} // }}}
