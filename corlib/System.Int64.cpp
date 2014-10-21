#include "pch.h"
#include "System.Int64.h"
#include "System.NumberFormatter.h"

namespace System
  {
  Int64::Int64()
    :_value()
    {
    }
  Int64::Int64(int64 const& value)
    :_value(value)
    {
    }
  Int64::~Int64()
    {
    }
  Int64& Int64::operator = (int64 const& value)
    {
    _value = value;
    return *this;
    }
  Int64::operator int64 const& () const
    {
    return _value;
    }
  String Int64::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Int64::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }
  uint32 Int64::GetHashCode()
    {
    return abs((int32)(_value & 0xffffffff) ^ (int32)(_value >> 32));
    }
  }
