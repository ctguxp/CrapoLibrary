// Perch Lake Computer System

#include "pch.h"
#include "System.Math.h"
#include "System.Single.h"
#include <cmath>


namespace System
  {
  const double Math::PI = 3.14159265358979323846;

  Math::Math()
    {
    }
  Math::~Math()
    {
    }
  byte Math::Max(byte val1, byte val2)
    {
    return (val1 > val2)? val1: val2;
    }
  int Math::Max(int32 val1, int32 val2)
    {
    return (val1 > val2)? val1: val2;
    }
  uintptr Math::Max(uintptr val1, uintptr val2)
    {
    return (val1 > val2)? val1: val2;
    }
  int Math::Min(int val1, int val2)
    {
    return (val1 < val2)? val1: val2;
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
