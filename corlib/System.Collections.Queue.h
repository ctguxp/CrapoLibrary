#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class Queue : public ICollection
      {
      private:
        int32           _head;   // points to the first used slot
        int32           _size;
        int32           _tail;
        int32           _growFactor;
        int32           _version;
        Array<GCObject> _array;
      public:
        Queue(int32 capacity = 32, float growFactor = 2.0f);
        Queue(const Queue& /*queue*/);
        virtual ~Queue();
        virtual void Clear(); 
        virtual int32 Count();
        virtual bool Contains(Object* /*obj*/);
        virtual bool IsSynchronized();
        virtual IEnumerator* GetEnumerator();
        virtual void Enqueue(GCObject& /*obj*/);
        virtual GCObject Dequeue();
      private:
        void Grow(); 
      };
    }
  }
