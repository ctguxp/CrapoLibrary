#pragma once
#include "System.Threading.CriticalSection.h"

namespace System
  {
  namespace Threading
    {
    class CRAPOCOREDLL_API Lock
      {
      public:
        Lock(CriticalSection&);
        ~Lock();
        Lock& operator=(const Lock&);
      private:
        CriticalSection& _mutex;
      };
    }
  }

