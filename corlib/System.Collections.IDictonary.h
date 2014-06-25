#pragma once
#include "System.Collections.ICollection.h"
#include "System.Collections.IDictionaryEnumerator.h"

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
        virtual void Clear() = 0;
        virtual bool Contains(Object* key) = 0;
        virtual void Remove(Object* key) = 0;
        virtual IDictionaryEnumerator* GetEnumerator() = 0;
      };
    }
  }
