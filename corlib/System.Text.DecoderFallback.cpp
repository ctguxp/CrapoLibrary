// Perch Lake Computer System

#include "pch.h"
#include "System.Text.DecoderFallback.h"
#include "System.Text.DecoderReplacementFallback.h"

namespace System
  {
  namespace Text
    {
    GCDecoderFallback DecoderFallback::_replacement_fallback(new DecoderReplacementFallback(), false);
    GCDecoderFallback DecoderFallback::_standard_safe_fallback(new DecoderReplacementFallback(L"\uFFFD"), false);

    /// Default constructor
    DecoderFallback::DecoderFallback()
      {
      }
    // Destructor
    DecoderFallback::~DecoderFallback()
      {
      }
    int DecoderFallback::MaxCharCount()
      {
      return 0;
      }
    DecoderFallbackBuffer* DecoderFallback::CreateFallbackBuffer()
      {
      return nullptr;
      }
    DecoderFallback& DecoderFallback::ReplacementFallback()
      {
      return dynamic_cast<DecoderFallback&>(*_replacement_fallback.Get());
      }
    DecoderFallback& DecoderFallback::StandardSafeFallback()
      {
      return dynamic_cast<DecoderFallback&>(*_standard_safe_fallback.Get());
      }
    }
  }
