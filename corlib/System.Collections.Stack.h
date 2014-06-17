#pragma once
#include "System.Array.h"
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class Stack : public ICollection
      {
      enum
        {
        default_capacity = 16
        };
      public:
        Stack();
        virtual ~Stack();
        virtual bool Contains(Object* obj);
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        virtual void Push(Object* /*obj*/);
        virtual Object* Pop();
      private:
        void Resize(sizet ncapacity);
      private:
        sizet          _capacity;
        sizet          _count;
        int32          _current;
        sizet          _modCount;
#pragma warning(disable:4251)
        Array<Object*> _contents;
#pragma warning(default:4251)
      };
    }
  }
