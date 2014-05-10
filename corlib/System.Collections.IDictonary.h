#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class IDictionary : public ICollection
      {
      public:
        // Properties
        virtual bool IsFixedSize() = 0;
        virtual bool IsReadOnly() = 0;
        // Functions
        virtual void Add(Object* key, Object* value) = 0;
      };
    }
  }
