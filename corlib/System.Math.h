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
      static byte Max(byte, byte);
      static int Max(int, int);
      static uintptr Max(uintptr, uintptr);
      static int Min(int, int);
      static double Sqrt(double d);
      static double Round(double a);
    };
  }
