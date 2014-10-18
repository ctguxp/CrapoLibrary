#pragma once
#include "System.Collections.ICollection.h"
#include "Global.Memory.h"

using namespace Global;

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
        virtual GCObject& operator[](const int32 index) = 0;
        virtual sizet Add(GCObject& value) = 0;
        virtual void Clear() = 0;
        virtual bool Contains(GCObject& value) = 0;
        virtual int IndexOf(GCObject& value) = 0;
        virtual void Insert(int32 index, GCObject& value) = 0;
        virtual void Remove(GCObject& value) = 0;
        virtual void RemoveAt(int32 index) = 0;
      };
    }
  }
