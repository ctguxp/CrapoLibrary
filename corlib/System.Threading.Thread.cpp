// Perch Lake Computer System

#include "pch.h"
#include "System.Threading.Thread.h"
#include "System.Threading.ThreadState.h"
#include <process.h>

namespace System
  {
  namespace Threading
    {

    Thread Thread::_current_thread(CurrentInternalThread());

    Thread::Thread(ParameterizedThreadStart threadStart)
      :_paramThreadStart(threadStart)
      ,_start_obj(nullptr)
      ,_current_culture()
      ,_internal_thread(nullptr)
      {
      //LPTHREAD_START_ROUTINE;
      }
    Thread::Thread(InternalThread* it)
      :_paramThreadStart(nullptr)
      ,_start_obj(nullptr)
      ,_internal_thread(it)
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
    void Thread::Start(Object* parameter)
      {
      _start_obj = parameter;
      Start();
      }
    void Thread::Start() 
      {
      // propagate informations from the original thread to the new thread
      /*if (!ExecutionContext.IsFlowSuppressed ())
      ec_to_set = ExecutionContext.Capture ();
      Internal._serialized_principal = CurrentThread.Internal._serialized_principal;*/

      // Thread_internal creates and starts the new thread, 
      //if (Thread_internal((ThreadStart) StartInternal) == (IntPtr) 0)
        //throw new SystemException ("Thread creation failed.");
      uint32 threadid;
      HANDLE h = (HANDLE)_beginthreadex(nullptr, 0, _paramThreadStart,(void*)_start_obj
                                        ,CREATE_SUSPENDED, &threadid);
      if(h != NULL)
        {
        ConstructInternalThread();
        _internal_thread->SystemThreadHandle(h);
        }
      else
        {
        throw SystemException(L"Thread creation failed.");
        }
      }
    }
  }