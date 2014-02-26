#pragma once
#include "System.Array.h"

namespace System
  {
  class Convert;
  namespace Security
    {
    namespace Cryptography
      {
      class CRAPOCOREDLL_API Base64Helper
        {
        enum class BlockSize : int
          {
          Input = 3,
          Output = 4
          };
        public:
          Base64Helper();
          ~Base64Helper();
        private:
          static void TransformBlock(ByteArray& inputBuffer, int inputOffset, int inputCount, ByteArray& outputBuffer, int outputOffset);
          static ByteArray TransformFinalBlock(ByteArray& inputBuffer, int inputOffset, int inputCount);
          static ByteArray ToByteArray(wchar_t* start, int length, bool allowWhitespaceOnly);

        friend Convert;
        };
      }
    }
  }


