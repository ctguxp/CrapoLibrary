#pragma once

#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class IComparer
      {
      public:
        virtual int32 Compare(Object*, Object*) = 0;
      };
    }
  }
