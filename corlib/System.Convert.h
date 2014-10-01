// Perch Lake Computer System

#pragma once
#include "System.Array.h"
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Convert
    {
    public:
      Convert();
      ~Convert();
      static ByteArray FromBase64CharArray(CharArray& inArray, int offset, int length);
      static int ToBase64CharArray(ByteArray& inArray, int offsetIn, int length, CharArray& outArray, int offsetOut);
      static byte ToByte(int value);
    private:
      static ByteArray InternalFromBase64CharArray(CharArray& arr, int offset, int length);
    };
  }
