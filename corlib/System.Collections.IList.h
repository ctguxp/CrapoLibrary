#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class IList : public ICollection
      {
      public:
        virtual void Clear() = 0;
      };
    }
  }
