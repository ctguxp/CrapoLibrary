#include "pch.h"
#include "System.UInt64.h"
#include "System.NumberFormatter.h"

namespace System
  {

  UInt64::UInt64()
    {
    }
  UInt64::UInt64(uint64 const& value)
    :_value(value)
    {
    }
  UInt64::~UInt64()
    {
    }
  UInt64& UInt64::operator = (uint64 const& value)
    {
    _value = value;
    return *this;
    }
  UInt64::operator uint64 const& () const
    {
    return _value;
    }
  String UInt64::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  uint32 UInt64::GetHashCode()
    {
    return abs((int32)(_value & 0xffffffff) ^ (int32)(_value >> 32));
    }
  }