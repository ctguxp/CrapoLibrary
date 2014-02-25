// Perch Lake Computer System

#include "pch.h"
#include "System.Text.DecoderReplacementFallback.h"
#include "System.Text.DecoderReplacementFallbackBuffer.h"
#include "System.Exception.h"

namespace System
  {
  namespace Text
    {
    // Default constructor
    DecoderReplacementFallback::DecoderReplacementFallback(String replacement)
      :_replacement(replacement)
      {
      }
    // Destructor
    DecoderReplacementFallback::~DecoderReplacementFallback()
      {
      }
   String& DecoderReplacementFallback::DefaultString()
			{
      return _replacement;
      }
    int DecoderReplacementFallback::MaxCharCount()
      {
      return _replacement.Length();
      }
    DecoderFallbackBuffer* DecoderReplacementFallback::CreateFallbackBuffer()
      {
      DecoderReplacementFallbackBuffer* retval = new DecoderReplacementFallbackBuffer(*this);
      return (DecoderFallbackBuffer*)retval;
      }
    }
  }
