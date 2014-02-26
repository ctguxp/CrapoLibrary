#include "pch.h"
#include "System.Byte.h"
#include "System.NumberFormatter.h"

namespace System
  {
  Byte::Byte()
    :_value()
    {
    }
  Byte::Byte(byte const& value)
    :_value(value)
    {
    }
  Byte::~Byte()
    {
    }
  Byte& Byte::operator = (byte const& value)
    {
    _value = value;
    return *this;
    }
  Byte::operator byte const& () const
    {
    return _value;
    }
  String Byte::ToString()
    {
    return NumberFormatter::NumberToString((uint32)_value, nullptr);
    }
  uint32 Byte::GetHashCode()
    {
    return abs(_value);
    }
  }
