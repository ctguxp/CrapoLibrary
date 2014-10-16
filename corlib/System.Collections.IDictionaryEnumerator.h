#pragma once

#include "System.Object.h"
#include "System.Collections.IEnumerator.h"

namespace System
  {
  namespace Collections
    {
    class IDictionaryEnumerator : public IEnumerator
      {
      public:
        virtual GCObject Key() = 0;
		    virtual GCObject Value() = 0;
      };
    }
  }
