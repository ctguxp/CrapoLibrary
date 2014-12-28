// Perch Lake Computer System

#pragma once
#pragma warning(disable:4127)

#include "System.Array.h"
#include "System.Array.hpp"
#include "System.Object.h"
#include "System.Exception.h"
#include "System.NotImplementedException.h"
#include "System.NotSupportedException.h"
#include "System.Collections.Generic.EqualityComparer.h"
#include "System.Collections.IComparer.h"
#include "Global.TypeFunctions.h"

struct QSortStack 
  {
  int32 high;
  int32 low;
  };

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
    if(_ptr == nullptr)
      return 0;

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
  void Array<T>::CheckComparerAvailable(Array<T>& keys, int32 low, int32 high)
    {
    // move null keys to beginning of array,
    // ensure that non-null keys implement IComparable
    for(int32 i = low; i < high; i++) {
      T key = keys[i];
      if(&key != nullptr) 
        {
        if(!::IsComparable(key))
          {
          throw SystemException(L"No IComparable interface found for type");
          }
        /*if(!(key is IComparable<K>) && !(key is IComparable))
        {
        string msg = Locale.GetText ("No IComparable<T> or IComparable interface found for type '{0}'.");
        throw new InvalidOperationException (String.Format (msg, key.GetType ()));
        }*/  
        }
      }
    }

  template<class T>
  GCObject Array<T>::GetValueImpl(int32 idx)
    {
    return GetNewObject(_ptr[idx]);
    }

  template<class T>
  void Array<T>::SetValueImpl(Object* obj, int32 idx)
    {
    Char* c = ::GetCharType(obj);
    if(c != nullptr)
      {
      SetCharArrayValue((wchar_t*)(_ptr + idx), *c);
      return;
      }
    String* s = ::GetStringType(obj);
    if(s != nullptr)
      {
      SetStringArray((String*)_ptr + idx, *s);
      return;
      }
    throw NotImplementedException();
    }

  template<class T>
  void Array<T>::Sort(Array<T>& arr, int32 index, int32 length)
    {
    Sort(arr, index, length, nullptr);
    }

  template<class T>
  void Array<T>::Sort(Array<T>& arr, int32 index, int32 length, Collections::IComparer* comparer)
    {
    if(&arr == nullptr)
      throw ArgumentNullException(L"array");

    /*if (array.Rank > 1)
    throw new RankException (Locale.GetText ("Only single dimension arrays are supported."));*/

    if(index < (int32)arr.GetLowerBound())
      throw ArgumentOutOfRangeException(L"index");

    if(length < 0)
      throw ArgumentOutOfRangeException(L"length", L"Value has to be >= 0.");

    if((int32)arr.Length() - ((int32)arr.GetLowerBound() + index) < length)
      throw ArgumentException ();

    SortImpl(arr, nullptr, index, length, comparer);
    }

  template<class T>
  void Array<T>::SortImpl(Array<T>& keys, Array<Object>* items, int32 index, int32 length, Collections::IComparer* comparer)
    {
    if(length <= 1)
      return;

    int32 low = index;
    int32 high = index + length - 1;

    if(comparer == nullptr)
      CheckComparerAvailable(keys, low, high);

    try 
      {
      qsort(keys, items, low, high, comparer);
      } 
    catch(Exception& ex) 
      {
      //throw InvalidOperationException(L"The comparer threw an exception."), e);
      throw SystemException(L"The comparer threw an exception.", ex);
      }				
    }

  template<class T>
  void Array<T>::qsort(Array<T>& keys, Array<Object>* items, int32 low0, int32 high0, Collections::IComparer* comparer)
    {
    QSortStack stack[32];
    const int QSORT_THRESHOLD = 7;
    int32 high, low, mid, i, k;
    GCObject key;
    GCObject hi;
    GCObject lo;
    IComparable* cmp = nullptr;
    int32 sp = 1;

    // initialize our stack
    stack[0].high = high0;
    stack[0].low = low0;

    do 
      {
      // pop the stack
      sp--;
      high = stack[sp].high;
      low = stack[sp].low;

      if((low + QSORT_THRESHOLD) > high) 
        {
        // switch to insertion sort
        for(i = low + 1; i <= high; i++) 
          {
          for(k = i; k > low; k--) 
            {
            lo = keys.GetValueImpl(k - 1);
            hi = keys.GetValueImpl(k);
            if(comparer != nullptr) 
              {
              if(comparer->Compare(hi.Get(), lo.Get()) >= 0)
                break;
              } 
            else 
              {
              if(lo.Get() == nullptr)
                break;

              if(hi.Get() != nullptr) 
                {
                cmp = dynamic_cast<IComparable*>(hi.Get());
                if(cmp->CompareTo(*lo) >= 0)
                  break;
                }
              }

            swap(keys, items, k - 1, k);
            }
          }

        continue;
        }

      // calculate the middle element
      mid = low + ((high - low) / 2);

      // get the 3 keys
      key = keys.GetValueImpl(mid);
      hi = keys.GetValueImpl(high);
      lo = keys.GetValueImpl(low);

      // once we re-order the low, mid, and high elements to be in
      // ascending order, we'll use mid as our pivot.
      QSortArrange(keys, items, low, lo, mid, key, comparer);
      if(QSortArrange (keys, items, mid, key, high, hi, comparer))
        QSortArrange (keys, items, low, lo, mid, key, comparer);

      cmp = dynamic_cast<IComparable*>(hi.Get());

      // since we've already guaranteed that lo <= mid and mid <= hi,
      // we can skip comparing them again.
      k = high - 1;
      i = low + 1;

      do 
        {
        // Move the walls in
        if(comparer != nullptr) 
          {
          // find the first element with a value >= pivot value
          GCObject k1(keys.GetValueImpl(i));
          while(i < k && comparer->Compare(key.Get(), k1.Get()) > 0)
            i++;

          // find the last element with a value <= pivot value
          k1 = keys.GetValueImpl(k);
          while(k > i && comparer->Compare(key.Get(), k1.Get()) < 0)
            k--;
          } 
        else if (cmp != nullptr)
          {
          // find the first element with a value >= pivot value
          GCObject k1(keys.GetValueImpl(i));
          while(i < k && cmp->CompareTo(*k1) > 0)
            i++;

          // find the last element with a value <= pivot value
          k1 = keys.GetValueImpl(k);
          while (k > i && cmp->CompareTo(*k1) < 0)
            k--;
          } 
        else 
          {
          // This has the effect of moving the null values to the front if comparer is null
          GCObject k1(keys.GetValueImpl(i));
          while(i < k && k1.Get() == nullptr)
            i++;

          k1 = keys.GetValueImpl(k);
          while(k > i && k1.Get() != nullptr)
            k--;
          }

        if (k <= i)
          break;

        swap(keys, items, i, k);

        i++;
        k--;
        }while(true);

        // push our partitions onto the stack, largest first
        // (to make sure we don't run out of stack space)
        if((high - k) >= (k - low)) 
          {
          if ((k + 1) < high) 
            {
            stack[sp].high = high;
            stack[sp].low = k;
            sp++;
            }

          if ((k - 1) > low) 
            {
            stack[sp].high = k;
            stack[sp].low = low;
            sp++;
            }
          } 
        else 
          {
          if ((k - 1) > low) 
            {
            stack[sp].high = k;
            stack[sp].low = low;
            sp++;
            }

          if ((k + 1) < high) 
            {
            stack[sp].high = high;
            stack[sp].low = k;
            sp++;
            }
          }
      } while (sp > 0);
    }

  template<class T>
  bool Array<T>::QSortArrange(Array<T>& keys, Array<Object>* items, int32 lo, GCObject& v0, int32 hi, GCObject& v1, Collections::IComparer* comparer)
    {
    IComparable* cmp;
    GCObject tmp;

    if(comparer != nullptr) 
      {
      if(comparer->Compare(v1.Get(), v0.Get()) < 0) 
        {
        swap(keys, items, lo, hi);
        tmp = v0;
        v0 = v1;
        v1 = tmp;

        return true;
        }
      } 
    else if(v0.Get() != nullptr)
      {
      cmp = dynamic_cast<IComparable*>(v1.Get());

      if (v1.Get() == nullptr || cmp->CompareTo(*v0) < 0) 
        {
        swap(keys, items, lo, hi);
        tmp = v0;
        v0 = v1;
        v1 = tmp;

        return true;
        }
      }

    return false;
    }

  template<class T>
  void Array<T>::swap(Array<T>& keys, Array<Object>* items, int32 i, int32 j)
    {
    GCObject tmp1(keys.GetValueImpl(i));
    GCObject tmp2(keys.GetValueImpl(j));
    keys.SetValueImpl(tmp2.Get(), i);
    keys.SetValueImpl(tmp1.Get(), j);

    if(items != nullptr) 
      {
      /*tmp = items.GetValueImpl (i);
      items.SetValueImpl (items.GetValueImpl (j), i);
      items.SetValueImpl (tmp, j);*/
      throw NotImplementedException();
      }
    }

  template<class T>
  Array2D<T>::Array2D(int32 r, int32 c)
    :_numberOfRows(r)
    ,_numberOfColumns(c)
    ,_array(r * c)
    {
    }

  template<class T>
  T& Array2D<T>::Select(int32 i, int32 j)
    {
    if(i >= _numberOfRows)
      throw ArgumentOutOfRangeException(L"Invalid Row");
    if(j >= _numberOfColumns)
      throw ArgumentOutOfRangeException(L"Invalid Column");
    return _array[i * _numberOfColumns + j];
    }

  template<class T>
  typename Array2D<T>::Row Array2D<T>::operator [] (int32 row)
    {
    return Row(*this, row);
    }
  }