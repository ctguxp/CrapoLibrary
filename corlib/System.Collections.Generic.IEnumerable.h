#pragma once;
#include "System.Collections.Generic.IEnumerator.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template <class T>
      class IEnumerable
        {
        public:
          virtual IEnumerator<T>* GetEnumerator() = 0;
        };
      }
    }
  }