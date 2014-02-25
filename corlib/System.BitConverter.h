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
    private:
      static bool AmILittleEndian();
    };
  }
