#include "pch.h"
#include "System.Threading.CriticalSection.h"

namespace System
  {
  namespace Threading
    {
    CriticalSection::CriticalSection()
      {
      ::InitializeCriticalSection(&_critSection);
      }
    CriticalSection::~CriticalSection()
      {
      ::DeleteCriticalSection(&_critSection);
      }
    void CriticalSection::Acquire()
      {
      ::EnterCriticalSection(& _critSection);
      }
    void CriticalSection::Release()
      {
      ::LeaveCriticalSection(& _critSection);
      }
    }
  }