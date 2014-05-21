#pragma once
#include "System.h"

using namespace System;

namespace Crapo
  {
  namespace Security
    {
    class BitConverterLE
      {
      public:
        BitConverterLE();
        ~BitConverterLE();
        static ByteArray GetBytes(float /*value*/);
        static float ToSingle(ByteArray& value, int startIndex);
      private:
        static void UIntFromBytes(byte* dst, ByteArray& src, int startIndex);
        static ByteArray GetUIntBytes(byte *bytes);
      };
    }
  }
