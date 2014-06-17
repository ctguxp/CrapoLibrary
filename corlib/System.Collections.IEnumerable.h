#pragma once;
#include "System.Collections.IEnumerator.h"

namespace System
  {
  namespace Collections
    {
    class IEnumerable
      {
      public:
        virtual IEnumerator* GetEnumerator() = 0;
      };
    }
  }