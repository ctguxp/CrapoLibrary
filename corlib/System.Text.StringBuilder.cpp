// Perch Lake Computer System

#include "pch.h"
#include "System.Text.StringBuilder.h"
#include "System.Exception.h"
#include "System.Math.h"

namespace System
  {
  namespace Text
    {
    StringBuilder::StringBuilder()
      :_length(0)
      ,_maxCapacity(Int32::MaxValue)
      ,_cached_str()
      ,_str()
      {
      }
    StringBuilder::StringBuilder(String value, int startIndex, int length, int capacity)
      :_length(0)
      ,_maxCapacity(Int32::MaxValue)
      ,_cached_str()
      ,_str()
      {
      Initialize(value, startIndex, length != 0 ? length : value.Length(), capacity);
      }
    StringBuilder::StringBuilder(uint32 capacity, uint32 maxCapacity)
      :_length(0)
      ,_maxCapacity(maxCapacity)
      ,_cached_str()
      ,_str()
      {
      String value;
      Initialize(value, 0, 0, capacity);
      }
    StringBuilder::~StringBuilder()
      {
      }

    /*const wchar_t& StringBuilder::operator [] (uint32 idx) const
    {
    assert(idx < _length);
    if(idx >= _length)
    throw System::ArgumentOutOfRangeException();
    const wchar_t* p = _str.get_buffer();
    return p[idx];
    }

    wchar_t& StringBuilder::operator [] (uint32 idx)
    {
    assert(idx < _length);
    if(idx >= _length)
    throw System::ArgumentOutOfRangeException();
    wchar_t* p = (wchar_t*)_str.get_buffer();
    return p[idx];
    }*/

    int StringBuilder::Capacity()
      {
      if(_str.Length() == 0)
        return Math::Min(_maxCapacity, constDefaultCapacity);

      return _str.Length();
      }

    int StringBuilder::Length()
      {
      return (int)_length;
      }

    void StringBuilder::Length(uint32 value)
      {
      if(value > _maxCapacity)
        throw ArgumentOutOfRangeException();

      if(value == _length)
        return;

      if(value < _length)
        {
        // LAMESPEC:  The spec is unclear as to what to do
        // with the capacity when truncating the string.

        // Do as MS, keep the capacity

        // Make sure that we invalidate any cached string.
        InternalEnsureCapacity(value);
        _length = value;
        } 
      else
        {
        // Expand the capacity to the new length and
        // pad the string with NULL characters.
        Append(L'\0', value - _length);
        }
      }

    uint32 StringBuilder::MaxCapacity()
      {
			return _maxCapacity;
		  }

    StringBuilder& StringBuilder::Append(wchar_t value, int repeatCount) 
      {
      if(repeatCount < 0 )
        throw ArgumentOutOfRangeException();

      InternalEnsureCapacity(_length + repeatCount);

      for(int i = 0; i < repeatCount; i++)
        _str.InternalSetChar(_length++, value);

      return *this;
      }

    StringBuilder& StringBuilder::Append(CharArray& value) 
      {
      if(value.IsNull())
        return *this;

      uint32 needed_cap = _length + (int)value.Length();
      if(/*_cached_str.IsNull() || */(uint32)_str.Length() < needed_cap)
        InternalEnsureCapacity(needed_cap);

      String::CharCopy(_str, _length, value, 0, (int)value.Length());
      _length = needed_cap;

      return *this;
      } 

    StringBuilder& StringBuilder::Append(wchar_t value)
      {
      int needed_cap = _length + 1;
      if(!_cached_str || (int)_str.Length() < needed_cap)
        InternalEnsureCapacity(needed_cap);

      _str.InternalSetChar(_length, value);
      _length = needed_cap;

      return *this;
      }

    StringBuilder& StringBuilder::Append(const wchar_t* value)
      {
      if(value == nullptr)
        return *this;
      sizet len = String::WcsLen(value);
      int needed_cap = _length + (int)len;
      if((int)_str.Length() < needed_cap)
        InternalEnsureCapacity(needed_cap);

      String::CharCopy((string)(cstring)_str + _length, value, (int)len + 1);
      _length = needed_cap;

      return *this;
      }

    StringBuilder& StringBuilder::Append(String value)
      {
      if(Length() == 0 && value.Length() < (int)_maxCapacity && value.Length() > _str.Length())
        {
        _length = value.Length();
        _cached_str = value;
        _str = _cached_str;
        return *this;
        }

      int needed_cap = Length() + value.Length();
      if(_str.Length() < needed_cap)
        InternalEnsureCapacity(needed_cap);

      String::CharCopy(_str, _length, value, 0, value.Length() + 1);
      _length = needed_cap;
      return *this;
      }

    StringBuilder& StringBuilder::Append(String value, int startIndex, int count) 
      {
      if((count < 0 || startIndex < 0) || (startIndex > (int)value.Length() - count))
        throw ArgumentOutOfRangeException();

      int needed_cap = _length + count;
      if(/*!_cached_str.IsNull() || */(int)_str.Length() < needed_cap)
        InternalEnsureCapacity(needed_cap);

      String::CharCopy(_str, _length, value, startIndex, count);
      _length = needed_cap;

      return *this;
      }

    String StringBuilder::ToString()
      {
      if(_length == 0)
        return String::Empty();

      // If we only have a half-full buffer we return a new string.
      if(Length() < (_str.Length() >> 1) || (_str.Length() > Int32::MaxValue && _length <= Int32::MaxValue))
        {
        // use String.SubstringUnchecked instead of String.Substring
        // as the former is guaranteed to create a new string object
        _cached_str = _str.SubstringUnchecked(0, _length);
        return _cached_str;
        }

      _cached_str = _str;
      _str.InternalSetLength(_length);

      return _str;
      }

    void StringBuilder::InternalEnsureCapacity(uint32 size) 
      {
      using namespace System;
      if((int)size > _str.Length() || _cached_str == _str)
        {
        int capacity = (int)_str.Length();

        // Try double buffer, if that doesn't work, set the length as capacity
        if((int)size > capacity)
          {
          // The first time a string is appended, we just set _cached_str
          // and _str to it. This allows us to do some optimizations.
          // Below, we take this into account.
          if(_cached_str == _str && capacity < constDefaultCapacity)
            capacity = constDefaultCapacity;

          capacity = capacity << 1;
          if((int)size > capacity)
            capacity = (int)size;

          if (capacity >= Int32::MaxValue || capacity < 0)
            capacity = Int32::MaxValue;

          if (capacity > (int)_maxCapacity && size <= _maxCapacity)
            capacity = _maxCapacity;

          if (capacity > (int)_maxCapacity)
            throw ArgumentOutOfRangeException (L"size", L"capacity was less than the current size.");
          }

        String tmp = String::InternalAllocateStr(capacity + 1);
        if(_length > 0)
          String::CharCopy (tmp, 0, _str, 0, _length + 1);

        _str = tmp;
        }

      _cached_str = String();
      }

    void StringBuilder::Initialize(String& value, int startIndex, int length, int capacity)
      {
      // make sure startIndex is zero or positive
      if(startIndex < 0)
        throw ArgumentOutOfRangeException(L"startIndex", L"StartIndex cannot be less than zero.");
      // TODO : throw ArgumentOutOfRangeException(L"startIndex", startIndex, L"StartIndex cannot be less than zero.");

      // make sure length is zero or positive
      if(length < 0)
        throw ArgumentOutOfRangeException(L"length", L"Length cannot be less than zero.");
      // TODO : throw ArgumentOutOfRangeException(L"length", length, L"Length cannot be less than zero.");

      if(capacity < 0)
        throw ArgumentOutOfRangeException(L"capacity", L"capacity must be greater than zero.");
      // TODO : throw ArgumentOutOfRangeException(L"capacity", capacity, L"capacity must be greater than zero.");

      if(_maxCapacity < 1)
        throw ArgumentOutOfRangeException(L"maxCapacity", L"maxCapacity is less than one.");
      if(capacity > (int)_maxCapacity)
        throw ArgumentOutOfRangeException(L"capacity", L"Capacity exceeds maximum capacity.");

      // make sure startIndex and length give a valid substring of value
      // re-ordered to avoid possible integer overflow
      if(startIndex > value.Length() - length)
        throw ArgumentOutOfRangeException(L"startIndex", L"StartIndex and length must refer to a location within the string.");
      // TODO : throw new ArgumentOutOfRangeException(L"startIndex", startIndex, L"StartIndex and length must refer to a location within the string.");

      if(capacity == 0) 
        {
        if(_maxCapacity > constDefaultCapacity)
          capacity = constDefaultCapacity;
        }

      if(_str.Length() == 0)
        _str.InternalSetLength((length > capacity) ? length : capacity);
      if(length > 0)
        String::CharCopy(_str, 0, value, startIndex, length);

      _length = length;
      }

    }
  }
