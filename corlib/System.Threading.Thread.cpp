// Perch Lake Computer System

#include "pch.h"
#include "System.Threading.Thread.h"
#include "System.Threading.ThreadState.h"

namespace System
  {
  namespace Threading
    {

    Thread Thread::_current_thread(CurrentInternalThread());

    Thread::Thread()
      :_current_culture()
      ,_internal_thread(nullptr)
      {
      }
    Thread::Thread(InternalThread* it)
      :_internal_thread(it)
      {
      }
    Thread::~Thread()
      {
      if(_internal_thread != nullptr)
        {
        delete _internal_thread;
        _internal_thread = nullptr;
        }
      }
    InternalThread* Thread::Internal()
      {
      if(_internal_thread == nullptr)
        {
        ConstructInternalThread();
        }
      return _internal_thread;
      }
    bool Thread::IsAlive()
      {
      ThreadState state = Internal()->_state;
      return (state & ThreadState::Aborted) == ThreadState::Running && (state & ThreadState::Stopped) == ThreadState::Running && (state & ThreadState::Unstarted) == ThreadState::Running;
      }
    Globalization::CultureInfo& Thread::CurrentCulture()
      {
      if(_current_culture.Get() != nullptr)
        return *_current_culture.Get();
      _current_culture.Set(Globalization::CultureInfo::ConstructCurrentCulture());
      return *_current_culture.Get();
      }
    Thread& Thread::CurrentThread()
      {
      return Thread::_current_thread;
      }
    InternalThread* Thread::CurrentInternalThread()
      {
      return nullptr;
      }
    void Thread::ConstructInternalThread()
      {
      _internal_thread = new InternalThread();
      }
    }
  }