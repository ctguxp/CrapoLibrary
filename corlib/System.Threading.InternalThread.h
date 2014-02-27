// Perch Lake Computer System

#pragma once
#include "System.Threading.ThreadState.h"

namespace System
  {
  namespace Threading
    {
    class Thread;
    class CRAPOCOREDLL_API InternalThread
      {
      public:
        ~InternalThread();
      protected:
        InternalThread();
        void SystemThreadHandle(HANDLE handle);
        HANDLE SystemThreadHandle();
      private:
        ulong       _thread_id;
        ThreadState _state;
        HANDLE     _system_thread_handle;

      friend Thread;
      };
    }
  }
