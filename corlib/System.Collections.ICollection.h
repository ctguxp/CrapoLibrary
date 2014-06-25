// Perch Lake Computer System
// Golden on 6/24/2014

#pragma once
#include "System.Object.h"
#include "System.Collections.IEnumerable.h"

namespace System
  {
  namespace Collections
    {
    class ICollection : public IEnumerable
      {
      public:
        virtual sizet Count() = 0;
        virtual bool IsSynchronized() = 0;
        //virtual Object* SyncRoot() = 0;
        //virtual void CopyTo(Array<Object*> array, int index) = 0;
      };
    }
  }
