// Perch Lake Computer System
// Golden on 2/14/2014

#pragma once

#include "System.String.h"
#include "System.Text.EncoderFallbackBuffer.h"
#include "System.Text.EncoderFallback.h"

namespace System
  {
  namespace Text
    {
    /// This EncoderFallbackBuffer is simple. It ignores the input buffers.
    /// EncoderFallbackBuffer users could implement their own complex 
    /// fallback buffers.
    class EncoderReplacementFallbackBuffer : EncoderFallbackBuffer
      {
        String _replacement;
        int    _current;
        bool   _fallback_assigned;
      public:
        EncoderReplacementFallbackBuffer(EncoderFallback&);
        ~EncoderReplacementFallbackBuffer();
        virtual int Remaining() override;
        virtual bool Fallback(wchar_t, int) override;
        virtual bool Fallback(wchar_t, wchar_t, int) override;
        virtual wchar_t GetNextChar() override;
        virtual bool MovePrevious() override;
        virtual void Reset() override;
      private:
        bool Fallback(int);
      };
    }
  }
