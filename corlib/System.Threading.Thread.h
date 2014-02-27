// Perch Lake Computer System

#pragma once
#include "System.Threading.InternalThread.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Object.h"

#pragma warning(disable:4251)
namespace System
  {
  namespace Threading
    {
    typedef uint32(WINAPI* ParameterizedThreadStart)(void* argument);
    class CRAPOCOREDLL_API Thread
      {
      public:
        Thread(ParameterizedThreadStart threadStart);
        ~Thread();
        Globalization::CultureInfo& CurrentCulture();
        static Thread& CurrentThread();
        bool IsAlive();
        void Start();
        void Start(Object* parameter);
        void WaitForDeath();
      private:
        Thread(InternalThread*);
        InternalThread* Internal();
        static InternalThread* CurrentInternalThread();
        void ConstructInternalThread();
      private:
        ParameterizedThreadStart     _paramThreadStart;
        Object*                      _start_obj;
        Globalization::GCCultureInfo _current_culture;
        InternalThread*              _internal_thread;
        static Thread                _current_thread;
      };
    }
  }
#pragma warning(default:4251)
