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
{ return "tsk";
} // }}}
