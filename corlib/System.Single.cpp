#include "pch.h"
#include "System.Single.h"
#include "System.NumberFormatter.h"
#include "System.Double.h"
#include <limits>

namespace System
  {
  const float Single::NaN = std::numeric_limits<float>::signaling_NaN();
  const float Single::MaxValue = FLT_MAX;
  const float Single::MinValue = FLT_MIN;
  const double Single::MaxValueEpsilon = 3.6147112457961776e29;

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
    float mantissa = frexp(_value, &exp);
    return (uint32)(2 * fabs(mantissa) - 1) * ~0U;
    } 
  bool Single::IsNaN(float f)
    {
    int status = _isnan(f);
    return status == 0 ? false : true;
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
  float Single::Parse(String s, IFormatProvider* provider)
    {
    using namespace Globalization;
    double parsed_value = Double::Parse(s, (NumberStyles)((intptr)NumberStyles::Float | (intptr)NumberStyles::AllowThousands), provider);
    if(parsed_value - (double)Single::MaxValue > MaxValueEpsilon && (!Double::IsPositiveInfinity(parsed_value)))
      //throw OverflowException();
      throw SystemException(L"Overflow");

    return (float)parsed_value;
    }
  }
