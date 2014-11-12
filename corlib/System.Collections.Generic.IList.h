#pragma once
#include "System.Collections.Generic.ICollection.h"
#include "Global.Memory.h"

using namespace Global;

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template<class T>
      class IList : public ICollection<T>
        {
        public:
          // Properties
          virtual bool IsFixedSize() = 0;
          virtual bool IsReadOnly() = 0;

          // Functions
          virtual T& operator[](const int32 index) = 0;
          virtual sizet Add(T& value) = 0;
          virtual void Clear() = 0;
          virtual bool Contains(T& value) = 0;
          virtual int32 IndexOf(T& value) = 0;
          virtual void Insert(int32 index, T& value) = 0;
          virtual void Remove(T& value) = 0;
          virtual void RemoveAt(int32 index) = 0;
        };
      }
    }
  }
