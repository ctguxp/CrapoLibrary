// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Text.DecoderFallbackBuffer.h"

namespace System
  {
  namespace Text
    {
    class DecoderFallback;
    typedef AutoPtr<DecoderFallback> GCDecoderFallback;
    class Encoding;
    class DecoderFallback
      {
      public:
        virtual ~DecoderFallback();
        virtual DecoderFallbackBuffer* CreateFallbackBuffer();
        virtual int MaxCharCount();
        static DecoderFallback& ReplacementFallback();
      protected:
        DecoderFallback();
        static DecoderFallback& StandardSafeFallback();
      protected:
        static GCDecoderFallback _replacement_fallback;
        static GCDecoderFallback _standard_safe_fallback;

      friend Encoding;
      };
    }
  }