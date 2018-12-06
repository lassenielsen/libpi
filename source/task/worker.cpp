#include <libpi/task/worker.hpp>
#include<thread>

using namespace std;
using namespace libpi::task;

bool _methods(Task *_task);

atomic<size_t> Worker::ActiveTasks(0);
size_t Worker::TargetTasks=std::thread::hardware_concurrency(); // Target number of active processes per worker
size_t Worker::Workers=std::thread::hardware_concurrency(); // Target number of active processes

list<Worker*> Worker::ourIdleWorkers;
atomic<size_t> Worker::ourIdleWorkersSize;
libpi::thread::Mutex Worker::ourIdleWorkersLock;

Worker::Worker() // {{{
: Value(NULL)
, myWaitLock(true)
, myGCFlag(false)
{
} // }}}

Worker::~Worker() // {{{
{
} // }}}
