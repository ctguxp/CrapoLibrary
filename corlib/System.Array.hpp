// Perch Lake Computer System

#pragma once

#include "System.Array.h"
#include "System.Array.hpp"
#include "System.Exception.h"
#include "System.NotImplementedException.h"
#include "System.NotSupportedException.h"
#include "System.Collections.Generic.EqualityComparer.h"

namespace System
  {

  // ------------------------------------------------------------------------
  /// Default constructor takes length or just creates empty array
  template<class T>
  Array<T>::Array(sizet len = 0)
    :_len(len)
    ,_base(0)
    ,_ptr(len > 0 ? new T[len] : nullptr)
    {
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// This constructor will create an array of T and initialize to any value
  /// You would like
  template<class T>
  Array<T>::Array(sizet len, T value)
    :_len(len)
    ,_base(0)
    ,_ptr(len > 0 ? new T[len] : nullptr)
    {
    if(_ptr != nullptr)
      {
      for(sizet i = 0; i < len; ++i)
        _ptr[i] = value;
      }
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// This constructor takes in a pointer to an array and its length
  /// This could be considered unsafe if used unwisely
  template<class T>
  Array<T>::Array(const T* arr, sizet len)
    :_len(len)
    ,_base(0)
    ,_ptr(len > 0 ? new T[len] : nullptr)
    {
    if(_ptr != nullptr)
      {
      for(sizet i =0; i < len; ++i)
        _ptr[i] = arr[i];
      }
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Copy constructor
  template<class T>
  Array<T>::Array(const Array<T>& arr)
    :_len(arr._len)
    ,_base(arr._base)
    {
    _ptr = (_len > 0) ? new T[_len] : nullptr;
    for(sizet i = 0; i < _len; ++i)
      _ptr[i] = arr._ptr[i];
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Assignment operator
  template<class T>
  Array<T>& Array<T>::operator = (const Array<T>& arr)
    {
    if(this == &arr)
      return *this;

    _len = arr._len;
    Free();
    _ptr = new T[_len];
    for(sizet i = 0; i < _len; ++i)
      _ptr[i] = arr._ptr[i];

    return *this;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  // Destructor
  template<class T>
  Array<T>::~Array()
    {
    Free();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Constant subscript operator
  template<class T>
  const T& Array<T>::operator[](int32 idx) const
    {
    sizet const offset = idx - _base;
    if(offset >= _len)
      throw ArgumentOutOfRangeException();
    return _ptr[offset];
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Subscript operator
  template<class T>
  T& Array<T>::operator[] (const int32 idx)
    {
    sizet const offset = idx - _base;
    if(offset >= _len)
      throw ArgumentOutOfRangeException();
    return _ptr[offset];
    }
  // ------------------------------------------------------------------------

  template<class T>
  void Array<T>::Base(sizet new_base)
    {
    _base = new_base;
    }

  template<class T>
  sizet Array<T>::GetLowerBound()
    {
    assert(_ptr != nullptr);
    if(_ptr == nullptr)
      throw SystemException(L"Array has not been initialized");
    return _base;
    }

  template<class T>
  sizet Array<T>::GetUpperBound()
    {
    assert(_ptr != nullptr);
    if(_ptr == nullptr)
      throw SystemException(L"Array has not been initialized");
    return _len - 1;  
    }

  // ------------------------------------------------------------------------
  /// Set Length Property (Will Grow or Shrink Array depending on new length)
  template<class T>
  void Array<T>::Length(sizet new_len)
    {
    T* new_ptr = new_len > 0 ? new T[new_len] : nullptr;
    sizet min = _len < new_len ? _len : new_len;
    for(sizet i = 0; i < min; ++i)
      new_ptr[i] = _ptr[i];
    Free();
    _ptr = new_ptr;
    _len = new_len;
    _base = 0;
    }
  // ------------------------------------------------------------------------

  template<class T>
  int32 Array<T>::Count()
    {
    return (int32)_len;
    }

  template<class T>
  bool Array<T>::IsSynchronized() 
    {
    return false;
    }

  template<class T>
  Collections::Generic::IEnumerator<T>* Array<T>::GetEnumerator()
    {
    return nullptr;
    }

  template<class T>
  bool Array<T>::IsFixedSize()
    {
    return true;
    }

  template<class T>
  bool Array<T>::IsReadOnly()
    {
    return false;
    }

  template<class T>
  sizet Array<T>::Add(T& /*value*/)
    {
    throw NotSupportedException();
    }

  template<class T>
  void Array<T>::Clear()
    {
    Array<T>::Clear(*this, (int32)GetLowerBound(), (int32)Length());
    }

  template<class T>
  int32 Array<T>::Rank()
    {
    return 1;
    }

  template<class T>
  bool Array<T>::Contains(T& value)
    {
    int32 length = (int32)Length();
    for(int32 i = 0; i < length; i++) 
      {
      if(value == _ptr[i])
        return true;
      }
    return false;
    }

  template<class T>
  int32 Array<T>::IndexOf(T& value)
    {
    int32 length = (int32)Length();
    for(int32 i = 0; i < length; i++) 
      {
      // array index may not be zero-based.  use lower bound
      if(value == _ptr[i])
        return i + (int32)GetLowerBound();
      }

    // lower bound may be MinValue
    return (int32)GetLowerBound() - 1;
    }

  template<class T>
  void Array<T>::Insert(int32 /*index*/, T& /*value*/)
    {
    throw NotSupportedException();
    }

  template<class T>
  void Array<T>::Remove(T& /*value*/)
    {
    throw NotSupportedException();
    }

  template<class T>
  void Array<T>::RemoveAt(int32 /*index*/)
    {
    throw NotSupportedException();
    }

  // ------------------------------------------------------------------------
  // Set all items in the array to same value
  template<class T>
  void Array<T>::Set(T value)
    {
    if(_ptr != nullptr)
      {
      for(sizet i = 0; i < _len; ++i)
        _ptr[i] = value;
      }
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Delete pointer and sets it to nullptr
  template<class T>
  void Array<T>::Free()
    {
    if(_ptr != nullptr)
      {
      delete [] _ptr;
      _ptr = nullptr;
      }
    }
  // ------------------------------------------------------------------------

  template<class T>
  void Array<T>::Copy(T* src, sizet srcIdx, T* dest, sizet destIdx, sizet length)
    {
    T* s1 = src + srcIdx;
    T* d1 = dest + destIdx;
    for(sizet i = 0; i < length; ++i)
      {
      d1[i] = s1[i];
      }
    }

  template<class T>
  void Array<T>::CopyByRef(Array<T>& src, sizet srcIdx, Array<T>& dest, sizet destIdx, sizet length)
    {
    T* s1 = src.ToPtr() + srcIdx;
    T* d1 = dest.ToPtr() + destIdx;
    for(sizet i = 0; i < length; ++i)
      {
      d1[i] = s1[i];
      }
    }

  template<class T>
  int Array<T>::IndexOf(Array<T*>& arr, T& value)
    {
    return IndexOf(arr, value, 0, arr.Length());
    }

  template<class T>
  int Array<T>::IndexOf(Array<T*>& arr, T& value, sizet startIndex)
    {
    return IndexOf(arr, value, startIndex, arr.Length() - startIndex);
    }

  template<class T>
  int Array<T>::IndexOf(Array<T*>& arr, T& value, sizet startIndex, sizet count)
    {
    using namespace Collections;
    // re-ordered to avoid possible integer overflow
    if(startIndex < arr.GetLowerBound() || startIndex - 1 > arr.GetUpperBound() - count)
      throw ArgumentOutOfRangeException ();

    return (int)Generic::EqualityComparer<T>::Default()->IndexOf(arr, value, (int)startIndex, (int)(startIndex + count) );
    }

  template<class T>
  void Array<T>::Clear(Array<T>& arr, int32 index, int32 length)
		{
			if(&arr == nullptr)
				throw ArgumentNullException(L"arr");
			if(length < 0)
				//TODO : throw IndexOutOfRangeException(L"length < 0");
        throw SystemException(L"Index of of range, length < 0");

			int32 low = (int32)arr.GetLowerBound();
			if(index < low)
				//throw IndexOutOfRangeException(L"index < lower bound");
        throw SystemException(L"Index of of range, index < lower bound");
			index = index - low;

			// re-ordered to avoid possible integer overflow
			if(index > (int32)arr.Length() - length)
				//throw IndexOutOfRangeException(L"index + length > size");
        throw SystemException(L"Index of of range, index + length > size");

			//TODO ClearInternal(arr, index, length);
		}

  template<class T>
  Array2D<T>::Array2D(sizet r, sizet c)
    :_numberOfRows(r)
    ,_numberOfColumns(c)
    ,_array(r * c)
    {
    }

  template<class T>
  T& Array2D<T>::Select(sizet i, sizet j)
    {
    if(i >= _numberOfRows)
      throw ArgumentOutOfRangeException(L"Invalid Row");
    if(j >= _numberOfColumns)
      throw ArgumentOutOfRangeException(L"Invalid Column");
    return _array[i * _numberOfColumns + j];
    }

  template<class T>
  typename Array2D<T>::Row Array2D<T>::operator [] (sizet row)
    {
    return Row(*this, row);
    }
  }