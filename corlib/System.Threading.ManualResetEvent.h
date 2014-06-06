#pragma once
#include "System.Threading.EventWaitHandle.h"
#include "AutoPtr.h"

namespace System
  {
  namespace Threading
    {
    class CRAPOCOREDLL_API ManualResetEvent : public EventWaitHandle
      {
      public:
        ManualResetEvent(bool initialState);
        ~ManualResetEvent();
      };
    typedef AutoPtr<ManualResetEvent> GCManualResetEvent;
    }
  }
