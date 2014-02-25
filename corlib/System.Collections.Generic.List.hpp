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
      List<T>::List(uintptr capacity)
        {
        _items.Length(capacity);
        }

      template<class T>
      const T& List<T>::operator [] (sizet idx) const
        {
        return _items[idx];
        }

      template<class T>
      T& List<T>::operator [] (sizet idx)
        {
        return _items[idx];
        }

      template<class T>
      void List<T>::Add(T item)
        {
        // If we check to see if we need to grow before trying to grow
        // we can speed things up by 25%
        if(_size == _items.Length())
          GrowIfNeeded(1);
        _items[_size++] = item;
        _version++;
        }

      template<class T>
      void List<T>::GrowIfNeeded(uintptr newCount)
        {
        uintptr minimumSize = _size + newCount;
        if (minimumSize > _items.Length())
          Capacity(Math::Max(Math::Max(Capacity() * 2, (uintptr)DefaultCapacity), minimumSize));
        }

      template<class T>
      uintptr List<T>::Capacity() 
        {
        return _items.Length();
        }

      template<class T>
      void List<T>::Capacity(uintptr value) 
        {
        if(value < _size)
          throw ArgumentOutOfRangeException();

        _items.Length(value);
        }

      template<class T>
      uintptr List<T>::Count()
        {
        return _size;
        }

      template<class T>
      Array<T> List<T>::ToArray()
		    {
        Array<T> ret(_size);
        for(uintptr i = 0; i < _size; ++i)
          ret[i] = _items[i];
			  return ret;
		    }

      }
    }
  }