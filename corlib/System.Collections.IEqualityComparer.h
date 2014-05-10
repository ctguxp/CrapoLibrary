#pragma once

#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class IEqualityComparer
      {
      public:
        virtual bool Equals(Object* x, Object* y) = 0;
		    virtual int GetHashCode(Object* obj) = 0;
      };
    }
  }