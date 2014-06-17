#include "pch.h"
#include "System.Collections.ArrayList.h"
#include "System.Array.hpp"
#include "System.UInt32.h"

namespace System
  {
  namespace Collections
    {
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
        _items[i] = nullptr;
      }
    ArrayList::~ArrayList()
      {
      Free();
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
    sizet ArrayList::Add(Object* value)
      {
      if(_items.Length() <= _size /* same as _items.Length < _size + 1) */) 
        EnsureCapacity(_size + 1);

      _items[_size] = value;

      _version++;

      return (int)_size++;
      }
    void ArrayList::Insert(sizet index, Object* value)
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
    void ArrayList::Remove(Object* value) 
      {
      int x = IndexOf(value);

      if(x > -1) 
        {
        RemoveAt(x);
        }

      _version++;
      }
    bool ArrayList::Contains(Object* item)
      {
      return IndexOf(item, 0, _size) > -1;
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

      if(_items[index] != nullptr)
        {
        delete _items[index];
        _items[index] = nullptr;
        }
      Shift(index, -1);
      _size--;
      _version++;
      }
    int ArrayList::IndexOf(Object* value)
      {
      return IndexOf(value, 0);
      }
    int ArrayList::IndexOf(Object* value, int startIndex)
      {
      return IndexOf(value, startIndex, _size - startIndex);
      }
    int ArrayList::IndexOf(Object* value, sizet startIndex, sizet count) 
      {
      if(startIndex > _size) 
        {
        UInt32 si((uint32)startIndex);
        ThrowNewArgumentOutOfRangeException(L"startIndex", &si, L"Does not specify valid index.");
        }

      // re-ordered to avoid integer overflow
      if(startIndex > _size - count) 
        {
        // LAMESPEC: Every other method throws ArgumentException

        throw ArgumentOutOfRangeException(L"count", L"Start index and count do not specify a valid range.");
        }

      return Array<Object>::IndexOf(_items, *value, startIndex, count);
      }

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
        if(_items[i] != nullptr)
          {
          delete _items[i];
          _items[i] = nullptr;
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
          Array<Object*> newData;

          newLength = (_items.Length() > 0) ? _items.Length() << 1 : 1;

          while(newLength < _size + count) 
            {
            newLength <<= 1;
            }

          newData.Length(newLength);

          Array<Object*>::Copy(_items, 0, newData, 0, index);
          Array<Object*>::Copy(_items, index, newData, index + count, _size - index);

          _items = newData;
          }
        else 
          {
          Array<Object*>::Copy(_items, index, _items, index + count, _size - index);
          }
        }
      else if (count < 0) 
        {
        // Remember count is negative so this is actually index + (-count)

        int x = (int32)index - count ;

        Array<Object*>::Copy(_items, x, _items, index, _size - x);
        }
      }
    }
  }
