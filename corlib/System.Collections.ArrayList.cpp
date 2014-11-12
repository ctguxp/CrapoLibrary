#include "pch.h"
#include "System.Collections.ArrayList.h"
#include "System.Array.hpp"
#include "System.Int32.h"
#include "System.Exception.h"

namespace System
  {
  namespace Collections
    {
    ArrayList::SimpleEnumerator::SimpleEnumerator(ArrayList& list)
      :_index(-1)
      ,_version(list._version)
      ,_list(list)
      ,_currentElement()
      {
      }

    ArrayList::SimpleEnumerator::~SimpleEnumerator()
      {
      }

    GCObject& ArrayList::SimpleEnumerator::Current()
      {
      if(_currentElement.Get() == nullptr)
        {
        if(_index == -1)
          //throw new InvalidOperationException(L"Enumerator not started");
            throw SystemException(L"InvalidOperationException Enumerator not started");
        else
          //throw new InvalidOperationException(L"Enumerator ended");
          throw SystemException(L"InvalidOperationException Enumerator ended");						 
        }

      return _currentElement;
      }

    bool ArrayList::SimpleEnumerator::MoveNext()
      {
      if(_version != _list._version)
        //throw new InvalidOperationException("List has changed.");
          throw SystemException(L"InvalidOperationException List has changed.");

      if (++_index < (int32)_list.Count()) 
        {
        _currentElement = _list[_index];
        return true;
        } 
      else 
        {
        _currentElement.Reset();
        return false;
        }
      }

    void ArrayList::SimpleEnumerator::Reset()
      {
      if(_version != _list._version)
        //throw new InvalidOperationException("List has changed.");
          throw SystemException(L"InvalidOperationException List has changed.");

      _currentElement.Reset();
      _index = -1;
      }


    ArrayList::ArrayList()
      :_size(0)
      ,_version(0)
      ,_items()
      {
      }

    ArrayList::ArrayList(int32 capacity)
      :_size(0)
      ,_version(0)
      ,_items(0)
      {

      if(capacity < 0) 
			{
				Int32 cap(capacity);
        ThrowNewArgumentOutOfRangeException(L"capacity", &cap, L"The initial capacity can't be smaller than zero.");
			}
      
      if(capacity == 0)
      {
      _size = DefaultInitialCapacity;
      _items.Length(DefaultInitialCapacity);
      }
      
      for(int32 i = 0; i < (int32)_items.Length(); ++i)
        _items[i] = GCObject();
      }

    ArrayList::ArrayList(ICollection* collection)
      :_size(collection != nullptr ? collection->Count() : 0)
      ,_version(0)
      ,_items()
      {
      //Array array;

			if(collection == nullptr) 
			{
				throw ArgumentNullException(L"c");
			}
						
			//array = c as Array;

			//if (array != null && array.Rank != 1) 
			//{
				//throw new RankException();
			//}

			//_items = new object[c.Count];
      _items.Length(collection->Count());

			AddRange(collection);
      }

    ArrayList::~ArrayList()
      {
      Free();
      }

    void ArrayList::Set(int32 index, GCObject& obj)
      {
      if(index >= _size) 
					//throw ArgumentOutOfRangeException(L"index", index, L"Index is less than 0 or more than or equal to the list count.");
          throw ArgumentOutOfRangeException(L"index", L"Index is less than 0 or more than or equal to the list count.");

				_items[index] = obj;
				_version++;
      }

    int32 ArrayList::Count()
      {
      return _size;
      }

    sizet ArrayList::Capacity()
      {
      return _items.Length();
      }
    void ArrayList::Capacity(int32 value) 
      {
      if(value < _size)
        { 
        Int32 v(value);
        ThrowNewArgumentOutOfRangeException(L"Capacity", &v, "Must be more than count.");
        }

      _items.Length(value);
      }

    bool ArrayList::IsSynchronized()
      {
      return false;
      }

    bool ArrayList::IsFixedSize()
      {
      return false;
      }

    bool ArrayList::IsReadOnly()
      {
      return false;
      }

    sizet ArrayList::Add(GCObject& value)
      {
      if((int32)_items.Length() <= _size /* same as _items.Length < _size + 1) */) 
        EnsureCapacity(_size + 1);

      _items[_size] = value;

      _version++;

      return (int)_size++;
      }

    void ArrayList::AddRange(ICollection* collection) 
		{
			InsertRange(_size, collection);
		}

    void ArrayList::Insert(int32 index, GCObject& value)
      {
      if(index > _size) 
        {
        Int32 i(index);
        ThrowNewArgumentOutOfRangeException("index", &i, "Index must be >= 0 and <= Count.");
        }

      Shift(index, 1);

      _items[index] = value;
      _size++;
      _version++;
      }

    void ArrayList::InsertRange(int32 index, ICollection* collection) 
		{
			if (collection == nullptr) 
			{
				throw ArgumentNullException(L"c");
			}

			if(index < 0 || index > _size) 
			{
				Int32 idx(index);
        ThrowNewArgumentOutOfRangeException(L"index", &idx, L"Index must be >= 0 and <= Count.");
			}

			int32 i = collection->Count();
			
			// Do a check here in case EnsureCapacity isn't inlined.

			if((int32)_items.Length() < _size + i) 
			{
				EnsureCapacity(_size + i);
			}

			if (index < _size) 
			{
				Array<GCObject>::Copy(_items.ToPtr(), index, _items.ToPtr(), index + i, _size - index);
			}
		
			// Handle inserting a range from a list to itself specially.

			//if(this == c->SyncRoot()) 
			//{
			//	// Copy range before the insert point.

			//	Array::Copy(_items, 0, _items, index, index);

			//	// Copy range after the insert point.

			//	Array::Copy(_items, index + i, _items, index << 1, _size - index);
			//}
			//else 
			//{
			//	collection->CopyTo(_items, index);
			//}
		
			_size += collection->Count();
			_version++;
		}

    void ArrayList::Remove(GCObject& value) 
      {
      int x = IndexOf(value);

      if(x > -1) 
        {
        RemoveAt(x);
        }

      _version++;
      }

    bool ArrayList::Contains(GCObject& /*item*/)
      {
      //TODO: return IndexOf(item, 0, _size) > -1;
      return false;
      }

    void ArrayList::Clear()
      {
      Free();
      _items.Length(0);
      _size = 0;
      _version++;
      }

    void ArrayList::RemoveAt(int32 index)
      {
      if(index >= _size) 
        {
        Int32 i(index);
        ThrowNewArgumentOutOfRangeException(L"index", &i, L"More than list count.");
        }

      if(_items[index].Get() != nullptr)
        {
        _items[index].Reset();
        }
      Shift(index, -1);
      _size--;
      _version++;
      }

    int ArrayList::IndexOf(GCObject& /*value*/)
      {
      //return IndexOf(value, 0);
      return 0;
      }

    //int ArrayList::IndexOf(GCObject& value, int startIndex)
    //  {
    //  return IndexOf(value, startIndex, _size - startIndex);
    //  }
    //int ArrayList::IndexOf(GCObject& value, sizet startIndex, sizet count) 
    //  {
    //  if(startIndex > _size) 
    //    {
    //    UInt32 si((uint32)startIndex);
    //    ThrowNewArgumentOutOfRangeException(L"startIndex", &si, L"Does not specify valid index.");
    //    }

    //  // re-ordered to avoid integer overflow
    //  if(startIndex > _size - count) 
    //    {
    //    // LAMESPEC: Every other method throws ArgumentException

    //    throw ArgumentOutOfRangeException(L"count", L"Start index and count do not specify a valid range.");
    //    }

    //  return Array<GCObject>::IndexOf(_items, value, startIndex, count);
    //  }

    IEnumerator* ArrayList::GetEnumerator() 
      {
      return new SimpleEnumerator(*this);
      }

    /*public virtual IEnumerator GetEnumerator(int index, int count) 
    {
    ArrayList.CheckRange(index, count, _size);

    return new ArrayListEnumerator(this, index, count);
    }*/

    // ------------------------------------------------------------------------
    /// Ensures that the list has the capacity to contain the given count by
    /// automatically expanding the capacity when required.
    void ArrayList::EnsureCapacity(sizet count) 
      {
      if(count <= _items.Length()) 
        {
        return;
        }

      sizet newLength;

      newLength = _items.Length() << 1;
      if(newLength == 0)
        newLength = DefaultInitialCapacity;

      while(newLength < count) 
        {
        newLength <<= 1;
        }

      _items.Length(newLength);
      }
    // ------------------------------------------------------------------------

    void ArrayList::Free()
      {
      for(int32 i = 0; i < _size; ++i)
        {
        if(_items[i].Get() != nullptr)
          {
          _items[i].Reset();
          }
        }
      }

    void ArrayList::ThrowNewArgumentOutOfRangeException(String name, Object*, String message)
      {
      //throw ArgumentOutOfRangeException(name, actual, message);
      throw ArgumentOutOfRangeException(name, message);
      }

    void ArrayList::Shift(sizet index, int count)
      {
      if(count > 0) 
        {
        if(_size + count > (int32)_items.Length()) 
          {
          int32 newLength;
          Array<GCObject> newData;

          newLength = ((int32)_items.Length() > 0) ? (int32)_items.Length() << 1 : 1;

          while(newLength < _size + count) 
            {
            newLength <<= 1;
            }

          newData.Length(newLength);

          Array<GCObject>::CopyByRef(_items, 0, newData, 0, index);
          Array<GCObject>::CopyByRef(_items, index, newData, index + count, _size - index);

          _items = newData;
          }
        else 
          {
          Array<GCObject>::CopyByRef(_items, index, _items, index + count, _size - index);
          }
        }
      else if (count < 0) 
        {
        // Remember count is negative so this is actually index + (-count)

        int x = (int32)index - count ;

        Array<GCObject>::CopyByRef(_items, x, _items, index, _size - x);
        }
      }
    }
  }
