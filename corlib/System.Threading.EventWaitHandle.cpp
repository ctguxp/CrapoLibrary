#include "pch.h"
#include "System.Threading.EventWaitHandle.h"
#include "System.Exception.h"

namespace System
  {
  namespace Threading
    {
    EventWaitHandle::EventWaitHandle(bool initialState, EventResetMode mode, String* name)
      :WaitHandle()
      {
      bool manual = IsManualReset(mode);
      if(name == nullptr)
        _waitHandle = ::CreateEvent(NULL, manual, initialState, NULL);
      else
        _waitHandle = ::CreateEvent(NULL, manual, initialState, (cstring)(*name));
      }
    EventWaitHandle::~EventWaitHandle()
      {
      }
    bool EventWaitHandle::IsManualReset(EventResetMode mode)
      {
      if((mode < EventResetMode::AutoReset) || (mode > EventResetMode::ManualReset))
        throw ArgumentException(L"mode");
      return (mode == EventResetMode::ManualReset);
      }
    bool EventWaitHandle::Set()
      {
      BOOL flag = ::SetEvent(_waitHandle);
      //if(!flag)
      //{
      //__Error.WinIOError();
      //}
      return flag == TRUE ? true : false;
      }
    }
  }
