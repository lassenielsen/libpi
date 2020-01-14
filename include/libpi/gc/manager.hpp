#pragma once
#include <libpi/task/worker.hpp>

namespace libpi
{
  namespace gc
  {
    enum ManagerMode
    { MANAGER_SAFEMODE=0,
      MANAGER_CONCURRENTMODE
    };

    class GCManager
    { public:
        static void Manage(std::vector<task::Worker*> &workers, size_t run_interval=100000, size_t wait_interval=100000, ManagerMode mode=MANAGER_CONCURRENTMODE);
    };
  }
}
