// Perch Lake Computer System

#pragma once

namespace System
  {
  class CRAPOCOREDLL_API BitConverter
    {
    public:
      static bool IsLittleEndian;
    public:
      BitConverter();
      ~BitConverter();
      static int64 DoubleToInt64Bits(double value);
    private:
      static bool AmILittleEndian();
    };
  }
