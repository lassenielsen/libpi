#pragma once

namespace libpi
{
  namespace gc
  {
    /*! Interface for registering objects to be managed by GC
     */
    class GCRegistrant
    { public:
        virtual void GCRegister(void *object)=0;
    };
  }
}
