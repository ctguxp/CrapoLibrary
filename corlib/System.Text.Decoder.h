#pragma once
#include "AutoPtr.h"
#include "System.Array.h"

namespace System
  {
  namespace Text
    {
    class Decoder
      {
      public:
        virtual ~Decoder();
        virtual int GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex) = 0;
      protected:
        Decoder();
      };
    typedef AutoPtr<Decoder> GCDecoder;
    }
  }
