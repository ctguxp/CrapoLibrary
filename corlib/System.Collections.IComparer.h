#pragma once

#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class IComparer
      {
      public:
        virtual bool Compare(Object*, Object*) = 0;
      };
    }
  }
