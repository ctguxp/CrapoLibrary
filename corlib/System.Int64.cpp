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
  bool Int64::Equals(Object* obj)
    {
    Int64 i;
    if(!Object::IsInstance(*obj, i))
      return false;
    Int64* o = static_cast<Int64*>(obj);
    return (*o)._value == _value;
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
  int Int64::CompareTo(Object& value)
    {
    if(&value == nullptr)
      return 1;

    Int64 test;
    if(!(Object::IsInstance(test, value)))
      throw ArgumentException(L"Value is not a System.Int64");

    Int64& lValue = static_cast<Int64&>(value);

    if(_value == lValue)
      return 0;

    return (_value < lValue) ? -1 : 1;
    }

  }
