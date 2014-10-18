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
        private:
          Enumerator(const Enumerator& e);
          Enumerator& operator=(const Enumerator& e);
        public:
          Enumerator(Stack&);
          virtual GCObject& Current() override;
          virtual bool MoveNext() override;
          virtual void Reset() override;
        private:
          Stack& _stack;
          sizet  _modCount;
          int32  _current;
        };
      public:
        Stack();
        Stack(int32 /*initialCapacity*/);
        virtual ~Stack();
        virtual bool Contains(Object* obj);
        virtual int32 Count() override;
        virtual bool IsSynchronized() override;
        virtual IEnumerator* GetEnumerator() override;
        virtual void Push(GCObject& /*obj*/);
        virtual GCObject Pop();
        virtual Object& Peek();
      private:
        void Resize(int32 ncapacity);
      private:
        int32          _capacity;
        int32          _count;
        int32          _current;
        sizet          _modCount;
#pragma warning(disable:4251)
        Array<GCObject> _contents;
#pragma warning(default:4251)
      };
    }
  }
