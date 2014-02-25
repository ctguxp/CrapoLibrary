// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Text.EncoderFallbackBuffer.h"

namespace System
  {
  namespace Text
    {
    class EncoderFallback;
    typedef AutoPtr<EncoderFallback> GCEncoderFallback;
    class Encoding;
    class EncoderFallback
      {
      public:
        virtual ~EncoderFallback();
        virtual EncoderFallbackBuffer* CreateFallbackBuffer();
        virtual int MaxCharCount();
        static EncoderFallback& ReplacementFallback();
      protected:
        EncoderFallback();
        static EncoderFallback& StandardSafeFallback();
      protected:
        static GCEncoderFallback _replacement_fallback;
        static GCEncoderFallback _standard_safe_fallback;

      friend Encoding;
      };
    }
  }
