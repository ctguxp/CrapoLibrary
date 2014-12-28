#pragma once
#include "System.Collections.Generic.IEnumerable.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template <class T>
      class ICollection : public IEnumerable<T>
        {
        public:
          virtual int32 Count() = 0;
          virtual bool IsSynchronized() = 0;
          //virtual T* SyncRoot() = 0;
          //virtual void CopyTo(Array<T*> array, int index) = 0;
        };
      }
    }
  }
