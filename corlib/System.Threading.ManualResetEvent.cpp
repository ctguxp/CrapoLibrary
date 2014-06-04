#include "pch.h"
#include "System.Threading.ManualResetEvent.h"

namespace System
  {
  namespace Threading
    {
    ManualResetEvent::ManualResetEvent(bool initialState) : EventWaitHandle(initialState, EventResetMode::ManualReset)
      {
      }
    ManualResetEvent::~ManualResetEvent()
      {
      }
    }
  }
