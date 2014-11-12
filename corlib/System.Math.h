// Perch Lake Computer System

#pragma once

namespace System
  {
  class CRAPOCOREDLL_API Math
    {
    public:
      static const double PI;
    public:
      Math();
      ~Math();
      static int32 DivRem(int32 a, int32 b, int32& result);
      static int64 DivRem(int64 a, int64 b, int64& result);
      static byte Max(byte, byte);
      static int32 Max(int32, int32);
      static uint64 Max(uint64, uint64);
      static int32 Min(int, int);
      static double Pow(double x, double y);
      static double Sqrt(double d);
      static double Round(double a);
    };
  }
