#include "pch.h"
#include "System.Threading.Lock.h"

namespace System
  {
  namespace Threading
    {
    Lock::Lock(Mutex& mutex)
      :_mutex(mutex)
      {
      _mutex.Acquire();
      }
    Lock::~Lock()
      {
      _mutex.Release();
      }
    Lock& Lock::operator=(const Lock& lock)
      {
      if(this == &lock)
        return *this;
      _mutex = lock._mutex;
      return *this;
      }
    }
  }
