// Perch Lake Computer System

#include "pch.h"
#include "System.Text.EncoderFallback.h"
#include "System.Text.EncoderReplacementFallback.h"

namespace System
  {
  namespace Text
    {
    GCEncoderFallback EncoderFallback::_replacement_fallback(new EncoderReplacementFallback(), false);
    GCEncoderFallback EncoderFallback::_standard_safe_fallback(new EncoderReplacementFallback(L"\uFFFD"), false);

    /// Default constructor
    EncoderFallback::EncoderFallback()
      {
      }
    // Destructor
    EncoderFallback::~EncoderFallback()
      {
      }
    int EncoderFallback::MaxCharCount()
      {
      return 0;
      }
    EncoderFallbackBuffer* EncoderFallback::CreateFallbackBuffer()
      {
      return nullptr;
      }
    EncoderFallback& EncoderFallback::ReplacementFallback()
      {
      return dynamic_cast<EncoderFallback&>(*_replacement_fallback.Get());
      }
    EncoderFallback& EncoderFallback::StandardSafeFallback()
      {
      return dynamic_cast<EncoderFallback&>(*_standard_safe_fallback.Get());
      }
    }
  }
