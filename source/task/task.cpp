#include <libpi/task/task.hpp>
#include <sys/mman.h>
#include <signal.h>
#include <thread>

using namespace std;
using namespace libpi::task;

libpi::thread::Channel Task::Tasks;
atomic<size_t> *_init_atasks() // {{{
{ atomic<size_t> *result=(std::atomic<size_t>*)mmap(NULL, sizeof(atomic<size_t>), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  (*result)=0;
  return result;
} // }}}
atomic<size_t> *Task::ActiveTasks=_init_atasks();
size_t Task::TargetTasks=std::thread::hardware_concurrency(); // Target number of active processes
size_t Task::MaxSteps=5; // Maximum number of steps before yielding

Task::~Task() // {{{
{
} // }}}

void Task::ToStream(std::ostream &dest) const // {{{
{ throw std::string("Task::ToStream not implemented");
} // }}}

std::string Task::GetType() const // {{{
{ return "sta";
} // }}}
