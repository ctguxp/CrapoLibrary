#pragma once
#include "System.Threading.EventWaitHandle.h"
#include "AutoPtr.h"

namespace System
  {
  namespace Threading
    {
    class ManualResetEvent : public EventWaitHandle
      {
      public:
        ManualResetEvent(bool initialState);
        ~ManualResetEvent();
      };
    typedef AutoPtr<ManualResetEvent> GCManualResetEvent;
    }
  }
