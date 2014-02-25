// Perch Lake Computer System
// Golden on 2/14/2014

#include "pch.h"
#include "System.Text.EncoderFallbackBuffer.h"

namespace System
  {
  namespace Text
    {
    // ------------------------------------------------------------------------
    /// Default constructor
    EncoderFallbackBuffer::EncoderFallbackBuffer()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Destructor
    EncoderFallbackBuffer::~EncoderFallbackBuffer()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Initializes all data and state information pertaining to this fallback buffer
    void EncoderFallbackBuffer::Reset()
      {
      while(GetNextChar() != L'\0')
        ;
      }
    // ------------------------------------------------------------------------

    }
  }
