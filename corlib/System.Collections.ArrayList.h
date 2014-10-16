#pragma once
#include "AutoPtr.h"
#include "System.Array.h"
#include "System.Object.h"
#include "System.Collections.IList.h"
#include "Global.Memory.h"

using namespace Global;

namespace System
  {
  namespace Collections
    {
    class CRAPOCOREDLL_API ArrayList : public IList
      {
      enum
        {
        DefaultInitialCapacity = 4
        };
      private:
        class SimpleEnumerator : public IEnumerator
          {
          private:
            SimpleEnumerator& operator=(SimpleEnumerator const&);
            SimpleEnumerator(SimpleEnumerator const&);
          public:
            SimpleEnumerator(ArrayList& list);
            virtual ~SimpleEnumerator();
            virtual GCObject Current() override;
            virtual bool MoveNext() override;
            virtual void Reset() override;
          private:
            int32      _index;
            int32      _version;
            ArrayList& _list;
            GCObject   _currentElement;
          };
      public:
        ArrayList();
        ArrayList(sizet capacity);
        virtual ~ArrayList();
        virtual GCObject& operator[](const sizet item) override { return _items[item]; }
        virtual sizet Capacity();
        void Set(sizet /*index*/, GCObject& /*obj*/);
        virtual void Capacity(sizet value);
        //virtual int IndexOf(GCObject& /*value*/, int /*startIndex*/);
        //virtual int IndexOf(GCObject& value, sizet startIndex, sizet count);
        // From IEnumerable
        virtual IEnumerator* GetEnumerator() override;
        //virtual IEnumerator* GetEnumerator(int index, int count) override;
        // From ICollection
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        // From IList
        virtual bool IsFixedSize() override;
        virtual bool IsReadOnly() override;
        virtual sizet Add(GCObject& value) override;
        virtual void Clear() override;
        virtual bool Contains(GCObject& /*item*/) override;
        virtual int IndexOf(GCObject& /*value*/) override;
        virtual void Insert(sizet index, GCObject& value) override;
        virtual void Remove(GCObject& value) override;
        virtual void RemoveAt(sizet index) override;
      private:
        void EnsureCapacity(sizet count);
        void Shift(sizet index, int count);
        void Free();
        static void ThrowNewArgumentOutOfRangeException(String name, Object* actual, String message);
      private:
        sizet           _size;
        int             _version;
#pragma warning(disable:4251)
        Array<GCObject>  _items;
#pragma warning(default:4251)
      };
    typedef SharedPtr<ArrayList> GCArrayList;
    }
  }
