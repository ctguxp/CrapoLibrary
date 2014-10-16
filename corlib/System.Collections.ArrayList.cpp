#include "pch.h"
#include "System.Collections.ArrayList.h"
#include "System.Array.hpp"
#include "System.UInt32.h"
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

    GCObject ArrayList::SimpleEnumerator::Current()
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
    ArrayList::ArrayList(sizet capacity)
      :_size(capacity != 0 ? capacity : 4)
      ,_version(0)
      ,_items(_size)
      {
      for(sizet i = 0; i < _items.Length(); ++i)
        _items[i] = GCObject();
      }
    ArrayList::~ArrayList()
      {
      Free();
      }
    void ArrayList::Set(sizet index, GCObject& obj)
      {
      if(index >= _size) 
					//throw ArgumentOutOfRangeException(L"index", index, L"Index is less than 0 or more than or equal to the list count.");
          throw ArgumentOutOfRangeException(L"index", L"Index is less than 0 or more than or equal to the list count.");

				_items[index] = obj;
				_version++;
      }
    size_t ArrayList::Count()
      {
      return _size;
      }
    sizet ArrayList::Capacity()
      {
      return _items.Length();
      }

    void ArrayList::Capacity(sizet value) 
      {
      if(value < _size)
        { 
        UInt32 v((uint32)value);
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
      if(_items.Length() <= _size /* same as _items.Length < _size + 1) */) 
        EnsureCapacity(_size + 1);

      _items[_size] = value;

      _version++;

      return (int)_size++;
      }
    void ArrayList::Insert(sizet index, GCObject& value)
      {
      if(index > _size) 
        {
        UInt32 i((uint32)index);
        ThrowNewArgumentOutOfRangeException("index", &i, "Index must be >= 0 and <= Count.");
        }

      Shift(index, 1);

      _items[index] = value;
      _size++;
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
    void ArrayList::RemoveAt(sizet index)
      {
      if(index >= _size) 
        {
        UInt32 i((uint32)index);
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
      for(sizet i = 0; i < _size; ++i)
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
        if(_size + count > _items.Length()) 
          {
          sizet newLength;
          Array<GCObject> newData;

          newLength = (_items.Length() > 0) ? _items.Length() << 1 : 1;

          while(newLength < _size + count) 
            {
            newLength <<= 1;
            }

          newData.Length(newLength);

          Array<GCObject>::Copy(_items, 0, newData, 0, index);
          Array<GCObject>::Copy(_items, index, newData, index + count, _size - index);

          _items = newData;
          }
        else 
          {
          Array<GCObject>::Copy(_items, index, _items, index + count, _size - index);
          }
        }
      else if (count < 0) 
        {
        // Remember count is negative so this is actually index + (-count)

        int x = (int32)index - count ;

        Array<GCObject>::Copy(_items, x, _items, index, _size - x);
        }
      }
    }
  }
