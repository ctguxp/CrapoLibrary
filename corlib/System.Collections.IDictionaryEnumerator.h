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
        virtual Object* Key() = 0;
		    virtual Object* Value() = 0;
      };
    }
  }
