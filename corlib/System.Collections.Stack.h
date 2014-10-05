#pragma once
#include "System.Array.h"
#include "System.Collections.ICollection.h"
#include "System.Collections.IEnumerable.h"

namespace System
  {
  namespace Collections
    {
    class CRAPOCOREDLL_API Stack : public ICollection
      {
      enum
        {
        default_capacity = 16
        };
      private:
        class Enumerator : public IEnumerator
        {
        enum
          {
          End = -1,
			    Begin = -2
          };
        public:
          Enumerator(Stack*);
          Enumerator(const Enumerator& e);
          Enumerator& operator=(const Enumerator& e);
          virtual GCObject Current() override;
          virtual bool MoveNext() override;
          virtual void Reset() override;
        private:
          Stack* _stack;
          sizet  _modCount;
          int32  _current;
        };
      public:
        Stack();
        Stack(sizet /*initialCapacity*/);
        virtual ~Stack();
        virtual bool Contains(Object* obj);
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        virtual IEnumerator* GetEnumerator() override;
        virtual void Push(Object* /*obj*/);
        virtual Object* Pop();
        virtual Object& Peek();
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
