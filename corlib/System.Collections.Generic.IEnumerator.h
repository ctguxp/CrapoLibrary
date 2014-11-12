#pragma once

#pragma once
#include "Global.Memory.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template<class T>
      class IEnumerator
        {
        public:
          virtual T& Current() = 0;
          virtual bool MoveNext() = 0;
          virtual void Reset() = 0;
        };
      }
    }
  }