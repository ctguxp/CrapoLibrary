#pragma once
#include "System.Threading.Mutex.h"

namespace System
  {
  namespace Threading
    {
    class Lock
      {
      public:
        Lock(Mutex&);
        ~Lock();
        Lock& operator=(const Lock&);
      private:
        Mutex& _mutex;
      };
    }
  }

