#pragma once
#include "System.Collections.IList.h"
#include "System.Collections.ArrayList.h"

namespace System
  {
  namespace Collections
    {
    class CRAPOCOREDLL_API CollectionBase : public IList
      {
      public:
        virtual ~CollectionBase();
        virtual Object& operator[](const sizet index) override;
        virtual bool IsFixedSize() override;
		    virtual bool IsReadOnly() override;
        virtual sizet Add(Object* value) override;
        virtual bool Contains(Object* value) override;
        virtual sizet Count() override;
        virtual void Clear() override;
        virtual int IndexOf(Object* value) override;
        virtual void Insert(sizet index, Object* value) override;
        virtual bool IsSynchronized() override;
        virtual void RemoveAt(sizet index) override;
        virtual void Remove(Object* value) override;
        virtual IEnumerator* GetEnumerator();
      protected:
        CollectionBase();
        CollectionBase(sizet capacity);
        sizet Capacity();
        void Capacity(sizet value); 
        ArrayList& InnerList();
        virtual void OnClear();
        virtual void OnClearComplete();
        virtual void OnInsert(sizet index, Object* value);
        virtual void OnInsertComplete(sizet index, Object* value);
        virtual void OnRemove(sizet index, Object* value);
		    virtual void OnRemoveComplete(sizet index, Object* value);
        virtual void OnValidate(Object* value);
      private:
        GCArrayList _list;
      };
    }
  }

