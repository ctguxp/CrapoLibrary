// Perch Lake Computer System

#pragma once
#include "System.Threading.InternalThread.h"
#include "System.Globalization.CultureInfo.h"

namespace System
  {
  namespace Threading
    {
    class Thread
      {
      public:
        Thread();
        ~Thread();
        Globalization::CultureInfo& CurrentCulture();
        static Thread& CurrentThread();
        bool IsAlive();
      private:
        Thread(InternalThread*);
        InternalThread* Internal();
        static InternalThread* CurrentInternalThread();
        void ConstructInternalThread();
      private:
        Globalization::GCCultureInfo _current_culture;
        InternalThread*              _internal_thread;
        static Thread                _current_thread;
      };
    }
  }
