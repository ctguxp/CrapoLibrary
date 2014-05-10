#pragma once

#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class ICollection
      {
      public:
        virtual int Count() = 0;
        virtual bool IsSynchronized() = 0;
      };
    }
  }
