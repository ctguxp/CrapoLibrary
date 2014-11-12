// Perch Lake Computer System

#include "pch.h"
#include "System.Math.h"
#include "System.Single.h"
#include "System.Double.h"
#include <cmath>
#include <limits>

namespace System
  {
  const double Math::PI = 3.14159265358979323846;

  Math::Math()
    {
    }

  Math::~Math()
    {
    }

  int32 Math::DivRem(int32 a, int32 b, int32& result)
    {
    result = (a % b);
    return(int32)(a / b);
    }

  int64 Math::DivRem(int64 a, int64 b, int64& result)
    {
    result = (a % b);
    return (int64)(a / b);
    }

  byte Math::Max(byte val1, byte val2)
    {
    return (val1 > val2)? val1: val2;
    }

  int Math::Max(int32 val1, int32 val2)
    {
    return (val1 > val2)? val1: val2;
    }
  uint64 Math::Max(uint64 val1, uint64 val2)
    {
    return (val1 > val2)? val1: val2;
    }

  int Math::Min(int val1, int val2)
    {
    return (val1 < val2)? val1: val2;
    }

  double Math::Pow(double x, double y)
    {
    double result;

    if(::_isnan(x) || _isnan(y)) 
      {
      return Double::NaN;
      }

    if((x == 1 || x == -1) && (y == HUGE_VAL || y == -HUGE_VAL)) 
      {
      return Double::NaN;
      }

    /* This code is for return the same results as MS.NET for certain
    * limit values */
    if(x < -9007199254740991.0) 
      {
      if(y > 9007199254740991.0)
        return HUGE_VAL;
      if(y < -9007199254740991.0)
        return 0;
      }

    result = pow(x, y);

    /* This code is for return the same results as MS.NET for certain
    * limit values */
    if(_isnan(result) && (x == -1.0) && ((y > 9007199254740991.0) || (y < -9007199254740991.0)))
      {
      return 1;
      }

    return (result == -0) ? 0: result;
    }

  double Math::Sqrt(double d)
    {
    if(d < 0)
      return Single::NaN;
    return sqrt(d);
    }

  double Math::Round(double a)
    {
    double int_part = floor(a);
    double dec_part = a - int_part;
    if (((dec_part == 0.5) && ((2.0 * ((int_part / 2.0) - floor(int_part / 2.0))) != 0.0)) || (dec_part > 0.5))
      {
      int_part++;
      }
    return int_part;
    }
  }
