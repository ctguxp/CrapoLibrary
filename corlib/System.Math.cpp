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
  int Math::Max(int val1, int val2)
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
  }
