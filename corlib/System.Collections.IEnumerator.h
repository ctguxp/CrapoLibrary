#pragma once
#include "System.Object.h"
#include "Global.Memory.h"

namespace System
  {
  namespace Collections
    {
    class IEnumerator
      {
      public:
        virtual GCObject& Current() = 0;
        virtual bool MoveNext() = 0;
        virtual void Reset() = 0;
      };
    typedef Global::SharedPtr<IEnumerator> GCIEnumerator;
    }
  }