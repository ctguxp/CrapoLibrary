#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class IList : public ICollection
      {
      public:
        // Properties
        virtual bool IsFixedSize() = 0;
		    virtual bool IsReadOnly() = 0;

        // Functions
        virtual int Add(Object* value) = 0;
        virtual void Clear() = 0;
        virtual bool Contains(Object* value) = 0;
      };
    }
  }
