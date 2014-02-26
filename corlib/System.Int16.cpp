// Perch Lake Computer System

#include "pch.h"
#include "System.Int16.h"
#include "System.NumberFormatter.h"

namespace System
  {
  Int16::Int16()
    :_value()
    {
    }
  Int16::Int16(int16 const& value)
    :_value(value)
    {
    }
  Int16::~Int16()
    {
    }
  Int16& Int16::operator = (int16 const& value)
    {
    _value = value;
    return *this;
    }
  Int16::operator int16 const& () const
    {
    return _value;
    }
  String Int16::ToString()
    {
    return NumberFormatter::NumberToString((int32)_value, nullptr);
    }
  uint32 Int16::GetHashCode()
    {
    return (uint32)abs(_value);
    }
  }
