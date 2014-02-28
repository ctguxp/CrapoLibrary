#pragma once
#include "System.Object.h"
#include "System.Threading.Thread.h"

namespace System
  {
  namespace Threading
    {
    class CRAPOCOREDLL_API ActiveObject : public Object
      {
      public:
        ActiveObject();
        virtual ~ActiveObject();
        void Kill();
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

