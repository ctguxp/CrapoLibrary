#pragma once
#include "AutoPtr.h"
#include "System.Array.h"
#include "System.Object.h"
#include "System.Collections.IList.h"

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
          public:
            SimpleEnumerator(ArrayList* list);
            virtual GCObject Current() override;
            virtual bool MoveNext() override;
            virtual void Reset() override;
          private:
            int32      _index;
            int32      _version;
            ArrayList* _list;
            Object*    _currentElement;
          };
      public:
        ArrayList();
        ArrayList(sizet capacity);
        virtual ~ArrayList();
        virtual Object& operator[](const sizet item) override { return (*_items[item]); }
        virtual sizet Capacity();
        void Set(sizet /*index*/, Object* /*obj*/);
        virtual void Capacity(sizet value);
        virtual int IndexOf(Object* /*value*/, int /*startIndex*/);
        virtual int IndexOf(Object* value, sizet startIndex, sizet count);
        // From IEnumerable
        virtual IEnumerator* GetEnumerator() override;
        //virtual IEnumerator* GetEnumerator(int index, int count) override;
        // From ICollection
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        // From IList
        virtual bool IsFixedSize() override;
        virtual bool IsReadOnly() override;
        virtual sizet Add(Object* value) override;
        virtual void Clear() override;
        virtual bool Contains(Object* /*item*/) override;
        virtual int IndexOf(Object* /*value*/) override;
        virtual void Insert(sizet index, Object* value) override;
        virtual void Remove(Object* value) override;
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
        Array<Object*>  _items;
#pragma warning(default:4251)
      };
    typedef AutoPtr<ArrayList> GCArrayList;
    }
  }
