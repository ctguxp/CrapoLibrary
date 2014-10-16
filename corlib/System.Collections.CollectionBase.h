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
        virtual GCObject& operator[](const sizet index) override;
        virtual bool IsFixedSize() override;
		    virtual bool IsReadOnly() override;
        virtual sizet Add(GCObject& value) override;
        virtual bool Contains(GCObject& value) override;
        virtual sizet Count() override;
        virtual void Clear() override;
        virtual int IndexOf(GCObject& value) override;
        virtual void Insert(sizet index, GCObject& value) override;
        virtual bool IsSynchronized() override;
        virtual void RemoveAt(sizet index) override;
        virtual void Remove(GCObject& value) override;
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
        virtual void OnRemove(sizet index, GCObject& value);
		    virtual void OnRemoveComplete(sizet index, GCObject& value);
        virtual void OnValidate(GCObject& value);
      private:
#pragma warning(disable:4251)
        GCArrayList _list;
#pragma warning(default:4251)
      };
    }
  }

