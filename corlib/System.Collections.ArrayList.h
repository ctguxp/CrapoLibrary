#pragma once
#include "System.Object.h"
#include "System.Collections.IList.h"

namespace System
  {
  namespace Collections
    {
    class ArrayList : public IList
      {
      public:
        ArrayList();
        virtual ~ArrayList();
        // From ICollection
        virtual int Count() override;
        virtual bool IsSynchronized() override;
        // From IList
        virtual void Clear() override;
      private:
        int            _size;
        int            _version;
        Array<Object*> _items;
      };
    }
  }
