#pragma once
#include "System.Collections.IList.h"
#include "System.Collections.ArrayList.h"

namespace System
  {
  namespace Collections
    {
    class CollectionBase : public IList
      {
      public:
        virtual ~CollectionBase();
      protected:
        CollectionBase();
        CollectionBase(sizet capacity);
      private:
        ArrayList _list;
      };
    }
  }

