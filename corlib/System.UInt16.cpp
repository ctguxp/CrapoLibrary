#include "pch.h"
#include "System.UInt16.h"
#include "System.NumberFormatter.h"

namespace System
  {
  UInt16::UInt16()
    :_value()
    {
    }
  UInt16::UInt16(uint16 const& value)
    :_value(value)
    {
    }
  UInt16::~UInt16()
    {
    }
  UInt16& UInt16::operator = (uint16 const& value)
    {
    _value = value;
    return *this;
    }
  UInt16::operator uint16 const& () const
    {
    return _value;
    }
  uint32 UInt16::GetHashCode()
    {
    return _value;
    }
  String UInt16::ToString()
    {
    return NumberFormatter::NumberToString((uint32)_value, nullptr);
    }
  }
