#include <libpi/task/task.hpp>

using namespace std;
using namespace libpi::task;

Task::~Task() // {{{
{
} // }}}

void Task::ToStream(std::ostream &dest) const // {{{
{ throw std::string("Task::ToStream not implemented");
} // }}}

std::string Task::GetType() const // {{{
{ return "sta";
} // }}}
