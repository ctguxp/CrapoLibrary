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

      return _size++;
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
      for(sizet i = 0; i < _items.Length(); ++i)
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
    }
  }
