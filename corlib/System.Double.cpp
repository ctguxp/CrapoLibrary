#include "pch.h"
#include "System.Double.h"
#include "System.NumberFormatter.h"
#include <float.h>
#include <math.h>

#define NAN ((double)0.0/(double)DBL_MIN)

namespace System
  {
  double Double::NaN = NAN;

  const double Double::MaxValue = DBL_MAX;
  const double Double::MinValue = DBL_MIN;

  Double::Double()
    :_value()
    {
    }
  Double::Double(double const& value)
    :_value(value)
    {
    }
  Double::~Double()
    {
    }
  Double& Double::operator = (double const& value)
    {
    _value = value;
    return *this;
    }
  Double::operator double const& () const
    {
    return _value;
    }
  uint32 Double::GetHashCode()
    {
    if(_value == 0)
      return 0;
    int exp;
    double mantissa = frexp(_value, &exp);
    return (uint32)(2 * fabs(mantissa) - 1) * ~0U;
    }
  String Double::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Double::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }
  String Double::ToString(String format)
    {
    return ToString(format, nullptr);
    }
  String Double::ToString(String format, IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(&format, _value, provider);
    }
  }
