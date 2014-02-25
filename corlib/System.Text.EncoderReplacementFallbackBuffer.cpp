// Perch Lake Computer System

#include "pch.h"
#include "System.Text.EncoderReplacementFallbackBuffer.h"
#include "System.Text.EncoderReplacementFallback.h"
#include "System.Char.h"
#include "System.Exception.h"

namespace System
  {
  namespace Text
    {
    // ------------------------------------------------------------------------
    /// Default constructor
    EncoderReplacementFallbackBuffer::EncoderReplacementFallbackBuffer(EncoderFallback& fallback)
      :_current(0)
      ,_fallback_assigned(false)
    {
    if(&fallback == nullptr)
      throw ArgumentNullException(L"fallback");
  
    EncoderReplacementFallback& fb = dynamic_cast<EncoderReplacementFallback&>(fallback);
    _replacement = fb.DefaultString();
    }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // Destructor
    EncoderReplacementFallbackBuffer::~EncoderReplacementFallbackBuffer()
    {
    }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Prepares the replacement fallback buffer to use the current replacement string.  
    bool EncoderReplacementFallbackBuffer::Fallback(wchar_t, int index)
      {
      return Fallback(index);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Indicates whether a replacement string can be used when an input surrogate pair cannot be encoded, or whether the surrogate pair can be ignored
    bool EncoderReplacementFallbackBuffer::Fallback(wchar_t, wchar_t, int index)
    {
    return Fallback(index);
    }
    // ------------------------------------------------------------------------
    
    // ------------------------------------------------------------------------
    /// Retrieves the next character in the replacement fallback buffer.
    wchar_t EncoderReplacementFallbackBuffer::GetNextChar()
      {
      if(_current >= (int)_replacement.Length())
        return System::Char::MinValue;
      return _replacement[_current++];
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Access the character position in the replacement fallback buffer prior to the current character position
    bool EncoderReplacementFallbackBuffer::MovePrevious()
      {
      if(_current == 0)
        return false;
      _current--;
      return true;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Initializes all internal state information and data in this instance of EncoderReplacementFallbackBuffer
    void EncoderReplacementFallbackBuffer::Reset()
      {
      _fallback_assigned = false;
      _current = 0;
      }
    // ------------------------------------------------------------------------
      
    // ------------------------------------------------------------------------
    /// Gets the number of characters in the replacement fallback buffer that remain to be processed.
    int EncoderReplacementFallbackBuffer::Remaining()
    {
      return _replacement.Length() - _current;
    }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private Fallback function
    /// hmm, what is this index for???
    bool EncoderReplacementFallbackBuffer::Fallback(int index)
      {
      if(_fallback_assigned && (Remaining() != 0))
        throw ArgumentException(L"Reentrant Fallback method invocation occurred. It might be because either this FallbackBuffer is incorrectly shared by multiple threads, invoked inside Encoding recursively, or Reset invocation is forgotten.");
      if(index < 0)
        throw ArgumentOutOfRangeException(L"index");
      _fallback_assigned = true;
      _current = 0;
      return _replacement.Length() > 0;
      }
    // ------------------------------------------------------------------------

    }
  }
