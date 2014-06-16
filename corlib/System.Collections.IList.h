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
        virtual Object& operator[](const sizet index) = 0;
        virtual sizet Add(Object* value) = 0;
        virtual void Clear() = 0;
        virtual bool Contains(Object* value) = 0;
        virtual int IndexOf(Object* value) = 0;
        virtual void Insert(sizet index, Object* value) = 0;
        virtual void Remove(Object* value) = 0;
        virtual void RemoveAt(sizet index) = 0;
      };
    }
  }
