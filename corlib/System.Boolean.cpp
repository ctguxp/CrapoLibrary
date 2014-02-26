#include "pch.h"
#include "System.Boolean.h"

namespace System
  {
  Boolean::Boolean()
    {
    }
  Boolean::Boolean(bool const& value)
    :_value(value)
    {

    }
  Boolean::~Boolean()
    {
    }
  Boolean& Boolean::operator = (bool const& value)
    {
    _value = value;
    return *this;
    }
  Boolean::operator bool const& () const
    {
    return _value;
    }
  uint32 Boolean::GetHashCode()
    {
    return _value ? 1 : 0;
    }
  String Boolean::ToString()
    {
    return _value ? String(L"True") : String(L"False");
    }
  }
