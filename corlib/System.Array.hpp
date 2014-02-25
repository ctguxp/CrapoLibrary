// Perch Lake Computer System

#pragma once

#include "System.Array.h"
#include "System.Exception.h"

namespace System
  {

  // ------------------------------------------------------------------------
  /// Default constructor takes length or just creates empty array
  template<class T>
  Array<T>::Array(sizet len = 0)
    :_len(len)
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
  Array<T>::Array(T* arr, sizet len)
    :_len(len)
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
    {
    _ptr = new T[_len];
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
  const T& Array<T>::operator[] (sizet idx) const
    {
    assert(idx < _len);
    if(idx >= _len)
      throw ArgumentOutOfRangeException();
    return _ptr[idx];
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Subscript operator
  template<class T>
  T& Array<T>::operator[] (sizet idx)
    {
    assert(idx < _len);
    if(idx >= _len)
      throw ArgumentOutOfRangeException();
    return _ptr[idx];
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Set Length Propery (Will Grow or Shrink Array depending on new length)
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
    }
  // ------------------------------------------------------------------------

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

  }