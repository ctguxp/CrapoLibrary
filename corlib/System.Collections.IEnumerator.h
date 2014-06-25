#pragma once
#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class IEnumerator
      {
      public:
        virtual GCObject Current() = 0;
        virtual bool MoveNext() = 0;
        virtual void Reset() = 0;
      };
    }
  }