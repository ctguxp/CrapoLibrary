// Perch Lake Computer System

#include "pch.h"
#include "System.Text.EncoderReplacementFallback.h"
#include "System.Text.EncoderReplacementFallbackBuffer.h"
#include "System.Exception.h"

namespace System
  {
  namespace Text
    {
    // ------------------------------------------------------------------------
    /// Default constructor
    EncoderReplacementFallback::EncoderReplacementFallback(String replacement)
      :_replacement(replacement)
      {
      /*if(replacement.IsNull())
        throw ArgumentNullException();*/
      //_replacement = replacement;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Destructor
    EncoderReplacementFallback::~EncoderReplacementFallback()
      {
      }
    // ------------------------------------------------------------------------
    
    // ------------------------------------------------------------------------
    /// Gets the replacement string that is the value of the EncoderReplacementFallback object.
    String& EncoderReplacementFallback::DefaultString()
      {
      return _replacement;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets the number of characters in the replacement string for the EncoderReplacementFallback object
    int EncoderReplacementFallback::MaxCharCount()
      {
      return _replacement.Length();
      }
    // ------------------------------------------------------------------------

    EncoderFallbackBuffer* EncoderReplacementFallback::CreateFallbackBuffer()
		  {
			EncoderReplacementFallbackBuffer* retval = new EncoderReplacementFallbackBuffer(*this);
      return (EncoderFallbackBuffer*)retval;
		  }

    }
  }
