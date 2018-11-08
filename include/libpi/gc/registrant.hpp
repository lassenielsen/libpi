#pragma once


namespace libpi
{
  class Value;

  namespace gc
  {
    /*! Interface for registering objects to be managed by GC
     */
    class GCRegistrant
    { public:
        virtual void GCRegister(libpi::Value *object)=0;
    };
  }
}
