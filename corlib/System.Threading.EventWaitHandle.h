#pragma once
#include "System.Threading.WaitHandle.h"
#include "System.String.h"

namespace System
  {
  namespace Threading
    {
    class EventWaitHandle : public WaitHandle
      {
      public:
        EventWaitHandle(bool /*initialState*/, EventResetMode /*mode*/, String* name = nullptr);
        ~EventWaitHandle();
        bool Set();
      private:
        static bool IsManualReset(EventResetMode mode);
      };
    }
  }
