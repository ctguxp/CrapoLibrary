// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Text.DecoderFallbackBuffer.h"
#include "System.Text.DecoderFallback.h"

namespace System
  {
  namespace Text
    {
    /// This DecoderFallbackBuffer is simple. It ignores the input buffers.
    /// DecoderFallbackBuffer users could implement their own complex 
    /// fallback buffers.
    class DecoderReplacementFallbackBuffer : public DecoderFallbackBuffer
      {
      private:
        String _replacement;
        int    _current;
        bool   _fallback_assigned;
      public:
        DecoderReplacementFallbackBuffer(DecoderFallback&);
        ~DecoderReplacementFallbackBuffer();
        virtual int Remaining() override;
        virtual bool Fallback(ByteArray&, int) override;
        virtual wchar_t GetNextChar() override;
        virtual bool MovePrevious() override;
        virtual void Reset() override;
      };
    }
  }