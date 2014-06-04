#include "pch.h"
#include "System.Threading.WaitHandle.h"

namespace System
  {
  namespace Threading
    {
    WaitHandle::WaitHandle()
      :_waitHandle(NULL)
      {
      }
    WaitHandle::~WaitHandle()
      {
      if(_waitHandle != NULL)
        {
        ::CloseHandle(_waitHandle);
        _waitHandle = NULL;
        }   
      }
    bool WaitHandle::WaitOne()
      {
      return WaitOne(-1, false);
      }
    bool WaitHandle::WaitOne(long timeout, bool exitContext)
      {
      DWORD ret = ::WaitForSingleObjectEx(_waitHandle, timeout, exitContext);
      if(ret == WAIT_FAILED)
        return false;
      else if(ret == WAIT_TIMEOUT)
        return false;
      return true;
      }

    }
  }
