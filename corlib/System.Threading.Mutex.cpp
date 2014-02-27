#include "pch.h"
#include "System.Threading.Mutex.h"

namespace System
  {
  namespace Threading
    {
    Mutex::Mutex()
      {
      ::InitializeCriticalSection(&_critSection);
      }
    Mutex::~Mutex()
      {
      ::DeleteCriticalSection(&_critSection);
      }
    void Mutex::Acquire()
      {
      ::EnterCriticalSection(& _critSection);
      }
    void Mutex::Release()
      {
      ::LeaveCriticalSection(& _critSection);
      }
    }
  }