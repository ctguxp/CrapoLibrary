// Perch Lake Computer System
// Golden on 2/16/2014

#pragma once
#include "AutoPtr.h"
#include "System.Array.h"

namespace System
  {
  namespace Text
    {
    class DecoderFallbackBuffer
      {
      public:
        virtual ~DecoderFallbackBuffer();
        virtual int Remaining() = 0;
        virtual bool Fallback(ByteArray&, int) = 0;
        virtual wchar_t GetNextChar() = 0;
        virtual bool MovePrevious() = 0;
        virtual void Reset();
      protected:
        DecoderFallbackBuffer();
      };
    typedef AutoPtr<DecoderFallbackBuffer> GCDecoderFallbackBuffer;
    }
  }
