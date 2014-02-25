// Perch Lake Computer System

#include "pch.h"
#include "System.Text.DecoderReplacementFallbackBuffer.h"
#include "System.Text.DecoderReplacementFallback.h"
#include "System.Exception.h"
#include "System.Char.h"

namespace System
  {
  namespace Text
    {
    // Default constructor
    DecoderReplacementFallbackBuffer::DecoderReplacementFallbackBuffer(DecoderFallback& fallback)
      :_replacement()
      ,_current(0)
      ,_fallback_assigned()
      {
      if(&fallback == nullptr)
				throw ArgumentNullException(L"fallback");
      DecoderReplacementFallback& fb = dynamic_cast<DecoderReplacementFallback&>(fallback);
      _replacement = fb.DefaultString();
      }
    // Destructor
    DecoderReplacementFallbackBuffer::~DecoderReplacementFallbackBuffer()
      {
      }
  
    int DecoderReplacementFallbackBuffer::Remaining()
      {
      return _fallback_assigned ? _replacement.Length() - _current : 0; 
      }

    bool DecoderReplacementFallbackBuffer::Fallback(System::ByteArray& bytesUnknown, int index)
      {
      if(bytesUnknown.IsNull())
				throw ArgumentNullException(L"bytesUnknown");
			if( _fallback_assigned && (Remaining() != 0) )
				throw ArgumentException(L"Reentrant Fallback method invocation occurred. It might be because either this FallbackBuffer is incorrectly shared by multiple threads, invoked inside Encoding recursively, or Reset invocation is forgotten.");
			if(index < 0 || (int)bytesUnknown.Length() < index)
				throw ArgumentOutOfRangeException(L"index");
			_fallback_assigned = true;
			_current = 0;
			return _replacement.Length() > 0;
      }

    wchar_t DecoderReplacementFallbackBuffer::GetNextChar()
      {
      if(!_fallback_assigned)
				return L'\0';
			if(_current >= (int)_replacement.Length())
				return Char::MinValue;
			return _replacement[_current++];
      }

    bool DecoderReplacementFallbackBuffer::MovePrevious()
      {
      if(_current == 0)
				return false;
			_current--;
			return true;
      }

    void DecoderReplacementFallbackBuffer::Reset()
      {
      _fallback_assigned = false;
			_current = 0;
      }
    }
  }
