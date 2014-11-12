#pragma once
#include "System.Collections.Generic.List.h"
#include "System.Math.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {

      template<class T>
      List<T>::List()
        :_items()
        ,_size(0)
        ,_version(0)
        {
        }

      template<class T>
      List<T>::List(int32 capacity)
        {
        _items.Length(capacity);
        }

      template<class T>
      const T& List<T>::operator [] (int32 idx) const
        {
        return _items[idx];
        }

      template<class T>
      T& List<T>::operator [] (int32 idx)
        {
        return _items[idx];
        }

      template<class T>
      void List<T>::Add(T item)
        {
        // If we check to see if we need to grow before trying to grow
        // we can speed things up by 25%
        if(_size == (int32)_items.Length())
          GrowIfNeeded(1);
        _items[_size++] = item;
        _version++;
        }

      template<class T>
      void List<T>::GrowIfNeeded(int32 newCount)
        {
        int32 minimumSize = _size + newCount;
        if (minimumSize > (int32)_items.Length())
          Capacity(Math::Max(Math::Max(Capacity() * 2, (int32)DefaultCapacity), minimumSize));
        }

      template<class T>
      int32 List<T>::Capacity() 
        {
        return (int32)_items.Length();
        }

      template<class T>
      void List<T>::Capacity(int32 value) 
        {
        if(value < _size)
          throw ArgumentOutOfRangeException();

        _items.Length(value);
        }

      template<class T>
      int32 List<T>::Count()
        {
        return _size;
        }

      template<class T>
      Array<T> List<T>::ToArray()
		    {
        Array<T> ret(_size);
        for(int32 i = 0; i < _size; ++i)
          ret[i] = _items[i];
			  return ret;
		    }

      }
    }
  }