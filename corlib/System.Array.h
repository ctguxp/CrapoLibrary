// Perch Lake Computer System
// Mark Mohr (markm11@charter.net)
//
// Excerpt from "Data Structures and Algorithms" by Bruno R Preiss.
// Probably the most common way to aggregate data is to use an array.  While the
// C++ programming language does indeed provide built-in support for arrays, that
// support is not without its shortcomings. Arrays in C++ are not first-class data types.
//
// According to Microsoft the .Net Array Class 
// Provides methods for creating, manipulating, searching, and sorting arrays,
// thereby serving as the base class for all arrays in the common language runtime.
//
// The purpose of this class is combining the above descriptions for the Crapo project

#pragma once
#include "System.Collections.Generic.IList.h"
#include "Global.Memory.h"

namespace System
  {
  class Object;
  typedef SharedPtr<Object> GCObject;
  namespace Collections
    {
    class IComparer;
    }
  template<class T>
  class Array : public System::Collections::Generic::IList<T>
    {
    private:
      sizet _len;  // Current length
      sizet _base;
      T*    _ptr;  // Pointer to array
    public:
      Array(sizet len = 0);
      Array(const T*, sizet);
      Array(sizet, T);
      Array(const Array<T>& arr);

      // Destructor
      virtual ~Array();
      Array<T>& operator = (const Array<T>& /*arr*/);
      const T& operator [] (int32 /*idx*/) const;
      virtual T& operator[](const int32 /*idx*/) override;

      bool IsNull() { return _ptr != nullptr ? false : true; }
      bool IsNull() const { return _ptr != nullptr ? false : true; }
      void Set(T value);
      T* ToPtr() { return _ptr; }
      const T* ToConstPtr() const { return _ptr; }
      
      sizet Base() const { return _base; }
      void Base(sizet new_base);
      virtual int32 Count() override;
      GCObject GetValueImpl(int32 idx);
      void SetValueImpl(Object* obj, int32 idx);
      virtual bool IsSynchronized() override;
      virtual Collections::Generic::IEnumerator<T>* GetEnumerator() override;
      virtual bool IsFixedSize() override;
      virtual bool IsReadOnly() override;
      virtual sizet Add(T& value) override;
      virtual void Clear() override;
      virtual bool Contains(T& value) override;
      virtual int32 IndexOf(T& value) override;
      virtual void Insert(int32 index, T& value) override;
      virtual void Remove(T& value) override;
      virtual void RemoveAt(int32 index) override;
      int Rank();
      sizet GetLowerBound();
      sizet GetUpperBound();
      sizet Length() const { return _len; }
      void Length(sizet new_len);

      static void Clear(Array<T>& arr, int32 index, int32 length);
      static void Copy(T*, sizet, T*, sizet, sizet);
      static void CopyByRef(Array<T>& /*src*/, sizet /*srcIndex*/, Array<T>&, sizet, sizet);
      static int IndexOf(Array<T*>& arr, T& value);
      static int IndexOf(Array<T*>& arr, T& value, sizet startIndex);
      static int IndexOf(Array<T*>& arr, T& value, sizet startIndex, sizet count);
      static void Sort(Array<T>& arr, int32 index, int32 length);
      static void Sort(Array<T>& arr, int32 index, int32 length, Collections::IComparer* comparer);

    private:
      void Free();
      static void CheckComparerAvailable(Array<T>& keys, int32 low, int32 high);
      static void SortImpl(Array<T>& keys, Array<Object>* items, int32 index, int32 length, Collections::IComparer* comparer);
      static bool QSortArrange(Array<T>& keys, Array<Object>* items, int32 lo, GCObject& v0, int32 hi, GCObject& v1, Collections::IComparer* comparer);
      static void qsort(Array<T>& keys, Array<Object>* items, int32 low0, int32 high0, Collections::IComparer* comparer);
      static void swap(Array<T>& keys, Array<Object>* items, int32 i, int32 j);
    };

  template <class T>
  class Array2D
  {
  protected:
    sizet    _numberOfRows;
    sizet    _numberOfColumns;
    Array<T> _array;
  public:
    class Row
    {
    private:
      Array2D& _array2D;
      sizet    _row;
    public:
      Row(Array2D& array2D, sizet row)
        :_array2D(array2D)
        ,_row(row)
        {
        }
      T& operator [] (sizet column) const
        {
        return _array2D.Select(_row, column);
        }
    };
    Array2D(sizet, sizet);
    T& Select(sizet, sizet);
    Row operator [] (sizet);
  };

  typedef Array<bool>    BoolArray;
  typedef Array<byte>    ByteArray;
  typedef Array<sbyte>   SByteArray;
  typedef Array<wchar_t> CharArray;
  typedef Array<int32>   IntArray;
  }
