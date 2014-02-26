#pragma once

namespace System
  {
  namespace Security
    {
    namespace Cryptography
      {
      class CRAPOCOREDLL_API Base64Constants
        {
        public:
          static const byte EncodeTable[64];
          static const byte DecodeTable[123];
        public:
          Base64Constants();
          ~Base64Constants();
        };
      }
    }
  }
