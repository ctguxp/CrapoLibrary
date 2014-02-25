#include "pch.h"
#include "System.Single.h"
#include "System.NumberFormatter.h"
#include <cfloat>
#include <cmath>

#define NAN ((double)0.0/(double)DBL_MIN)

namespace System
  {
  float Single::NaN = NAN;

  Single::Single()
    :_value()
    {
    }
  Single::Single(float const& value)
    :_value(value)
    {
    }
  Single::~Single()
    {
    }
  Single& Single::operator = (float const& value)
    {
    _value = value;
    return *this;
    }
  Single::operator float const& () const
    {
    return _value;
    }
  uint32 Single::GetHashCode()
    {
    if(_value == 0)
      return 0;
    int exp;
    float mantissa = std::frexp(_value, &exp);
    return (uint32)(2 * fabs(mantissa) - 1) * ~0U;
    }
  String Single::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Single::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }
  String Single::ToString(String format)
    {
    return ToString(format, nullptr);
    }
  String Single::ToString(String format, IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(&format, _value, provider);
    }
  }
