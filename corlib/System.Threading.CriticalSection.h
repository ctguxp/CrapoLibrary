#pragma once

namespace System
  {
  namespace Threading
    {
    class Lock;
    class CRAPOCOREDLL_API CriticalSection
      {
      public:
        CriticalSection();
        ~CriticalSection();
      private:
        void Acquire();
        void Release();
      private:
        CRITICAL_SECTION _critSection;

      friend Lock;
      };
    }
  }
