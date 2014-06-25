// Perch Lake Computer System
// Golden on 6/24/2014

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