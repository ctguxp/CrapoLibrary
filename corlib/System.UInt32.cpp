#include "pch.h"
#include "System.UInt32.h"
#include "System.NumberFormatter.h"

namespace System
  {
  UInt32::UInt32()
    :_value()
    {
    }
  UInt32::UInt32(uint32 const& value)
    :_value(value)
    {
    }
  UInt32::~UInt32()
    {
    }
  UInt32& UInt32::operator = (uint32 const& value)
    {
    _value = value;
    return *this;
    }
  UInt32::operator uint32 const& () const
    {
    return _value;
    }
  uint32 UInt32::GetHashCode()
    {
    return _value;
    }
  String UInt32::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  }
