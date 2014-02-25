// Perch Lake Computer System

#include "pch.h"
#include "System.Int32.h"
#include "System.NumberFormatter.h"

namespace System
  {
  Int32::Int32()
    :_value()
    {
    }
  Int32::Int32(int32 const& d)
    :_value(d)
    {
    }
  Int32::~Int32()
    {
    }
  Int32& Int32::operator = (int32 const& d)
    {
    _value = d;
    return *this;
    }
  Int32::operator int32 const& () const
    {
    return _value;
    }
  String Int32::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Int32::ToString(String& format)
    {
    return NumberFormatter::NumberToString(&format, _value, nullptr);
    }
  uint32 Int32::GetHashCode()
    {
    return (uint32)abs(_value);
    }
  }
