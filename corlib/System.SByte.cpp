#include "pch.h"
#include "System.SByte.h"
#include "System.NumberFormatter.h"

namespace System
  {
  SByte::SByte()
    :_value()
    {
    }
  SByte::SByte(sbyte const& value)
    :_value(value)
    {
    }
  SByte::~SByte()
    {
    }
  SByte& SByte::operator = (sbyte const& value)
    {
    _value = value;
    return *this;
    }
  SByte::operator sbyte const& () const
    {
    return _value;
    }
  String SByte::ToString()
    {
    return NumberFormatter::NumberToString((int32)_value, nullptr);
    }
  uint32 SByte::GetHashCode()
    {
    return abs(_value);
    }
  }
