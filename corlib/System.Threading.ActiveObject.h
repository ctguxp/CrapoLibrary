#pragma once
#include "System.Object.h"
#include "System.Threading.Thread.h"

namespace System
  {
  namespace Threading
    {
    class ActiveObject : public Object
      {
      public:
        ActiveObject();
        virtual ~ActiveObject();
      protected:
        virtual void InitThread () = 0;
        virtual void Run () = 0;
        virtual void FlushThread () = 0;
        static uint32 _stdcall ThreadEntry(void*);
      protected:
        bool   _isDying;
        Thread _thread;
      };
    }
  }

