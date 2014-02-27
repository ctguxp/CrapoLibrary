#pragma once

namespace System
  {
  namespace Threading
    {
    class Lock;
    class Mutex
      {
      public:
        Mutex();
        ~Mutex();
      private:
        void Acquire();
        void Release();
      private:
        CRITICAL_SECTION _critSection;

      friend Lock;
      };
    }
  }
