#include "pch.h"
#include "System.Collections.CollectionBase.h"
#include "System.Exception.h"

namespace System
  {
  namespace Collections
    {
    CollectionBase::CollectionBase()
      :_list()
      {
      }
    CollectionBase::CollectionBase(sizet capacity)
      :_list( new ArrayList(capacity) )
      {
      }
    CollectionBase::~CollectionBase()
      {
      }
    GCObject& CollectionBase::operator[](const sizet index)
      {
      return InnerList()[index];
      }
    sizet CollectionBase::Capacity()
      {
      if(_list.Get() == nullptr)
        _list.Reset(new ArrayList());

      return _list->Capacity();
      }
    void CollectionBase::Clear()
      { 
      OnClear();
      InnerList().Clear(); 
      OnClearComplete();
      }
    IEnumerator* CollectionBase::GetEnumerator()
      { 
      return InnerList().GetEnumerator();
      }
    bool CollectionBase::IsSynchronized()
      {
      return InnerList().IsSynchronized();
      }
    sizet CollectionBase::Count()
      { 
      return InnerList().Count(); 
      }
    bool CollectionBase::IsFixedSize() 
      { 
      return InnerList().IsFixedSize();
      }
    bool CollectionBase::IsReadOnly() 
      { 
      return InnerList().IsReadOnly();
      }
    sizet CollectionBase::Add(GCObject& value)
      {
      sizet newPosition;
      OnValidate(value);
      newPosition = InnerList().Count();
      OnInsert(newPosition, value.Get());
      InnerList().Add(value);
      try
        {
        OnInsertComplete(newPosition, value.Get());
        }
      catch(Exception&)
        {
        InnerList().RemoveAt(newPosition);
        throw;
        }

      return newPosition;
      }
    void CollectionBase::Insert(sizet index, GCObject& value)
      {
      OnValidate(value);
      OnInsert(index, value.Get());
      InnerList().Insert(index, value);
      try
        {
        OnInsertComplete(index, value.Get());
        }
      catch(Exception&)
        {
        InnerList().RemoveAt(index);
        throw;
        }
      }
    void CollectionBase::RemoveAt(sizet index) 
      {
      GCObject& objectToRemove = InnerList()[index];
      OnValidate(objectToRemove);
      OnRemove(index, objectToRemove);
      InnerList().RemoveAt(index);
      OnRemoveComplete(index, objectToRemove);
      }
    void CollectionBase::Remove(GCObject& value)
      {
      int removeIndex;
      OnValidate(value);
      removeIndex = InnerList().IndexOf(value);
      if(removeIndex == -1)
        throw ArgumentException(L"The element cannot be found.", L"value");
      OnRemove(removeIndex, value);
      InnerList().Remove(value);
      OnRemoveComplete(removeIndex, value);
      }
    bool CollectionBase::Contains(GCObject& value)
      {
      return InnerList().Contains(value);
      }
    int CollectionBase::IndexOf(GCObject& value)
      {
      return InnerList().IndexOf(value);
      }
    void CollectionBase::Capacity(sizet value)
      {
      if(_list.Get() == nullptr)
        _list.Reset(new ArrayList());

      _list->Capacity(value);
      }
    ArrayList& CollectionBase::InnerList()
      {
      if(_list.Get() == nullptr)
        _list.Reset(new ArrayList());
      return (*_list.Get());
      }
    void CollectionBase::OnClear()
      {
      }
    void CollectionBase::OnClearComplete()
      {
      }
    void CollectionBase::OnRemove(sizet, GCObject&)
      {
      }
    void CollectionBase::OnRemoveComplete(sizet, GCObject&)
      {
      }
    void CollectionBase::OnInsert(sizet, Object*)
      {
      }
    void CollectionBase::OnInsertComplete(sizet, Object*)
      {
      }
    void CollectionBase::OnValidate(GCObject& value)
      {
      if(nullptr == value.Get())
        throw ArgumentNullException(L"CollectionBase.OnValidate: Invalid parameter value passed to method: null");
      }
    }
  }
