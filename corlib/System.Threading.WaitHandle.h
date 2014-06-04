#pragma once
#include "System.Threading.EventResetMode.h"

namespace System
  {
  namespace Threading
    {
    class CRAPOCOREDLL_API WaitHandle
      {
      public:
        virtual ~WaitHandle();
        virtual bool WaitOne();
      protected:
        WaitHandle();
      private:
        bool WaitOne(long timeout, bool exitContext);
      protected:
        HANDLE _waitHandle;
      };
    }
  }
