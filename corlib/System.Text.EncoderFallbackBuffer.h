// Perch Lake Computer System
// Golden on 2/14/2014

#pragma once
#include "AutoPtr.h"

namespace System
  {
  namespace Text
    {
    class EncoderFallbackBuffer
      {
      public:
        virtual ~EncoderFallbackBuffer();
        virtual int Remaining() = 0;
        virtual bool Fallback(wchar_t, int) = 0;
        virtual bool Fallback(wchar_t, wchar_t, int) = 0;
        virtual wchar_t GetNextChar() = 0;
        virtual bool MovePrevious() = 0;
        virtual void Reset();
      protected:
        EncoderFallbackBuffer();
      };
    typedef AutoPtr<EncoderFallbackBuffer> GCEncoderFallbackBuffer;
    }
  }
