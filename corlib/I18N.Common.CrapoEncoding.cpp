// Perch Lake Computer System

#include "pch.h"
#include "I18N.Common.CrapoEncoding.h"
#include "System.Exception.h"
#include "System.Char.h"
#include "System.Text.EncoderFallback.h"

using namespace System;

namespace I18N
  {
  namespace Common
    {

    // ------------------------------------------------------------------------
    /// Default Constructor
    CrapoEncoding::CrapoEncoding(int codePage, int windowsCodePage)
      :Text::Encoding(codePage)
      ,_win_code_page(windowsCodePage)
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Destructor
    CrapoEncoding::~CrapoEncoding()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // GetByteCount function
    int CrapoEncoding::GetByteCount(CharArray& chars, int index, int count)
      {
      if(chars.IsNull())
        throw ArgumentNullException(L"chars");
      if(index < 0 || index > (int)chars.Length())
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
      if(count < 0 || count > ((int)chars.Length() - index))
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");

      if(count == 0)
        return 0;

      wchar_t* cptr = chars.ToPtr();
      return GetByteCountImpl(cptr + index, count);
      }
    int CrapoEncoding::GetByteCount(String& s)
      {
      return Text::Encoding::GetByteCount(s);
      }
    int CrapoEncoding::GetByteCount(CharArray& chars)
      {
      return Text::Encoding::GetByteCount(chars);
      }
    int CrapoEncoding::GetByteCount(wchar_t* chars, int count)
      {
      return GetByteCountImpl(chars, count);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // GetByteCount function
    int CrapoEncoding::GetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex)
      {
      if(chars.IsNull())
        throw ArgumentNullException(L"chars");
      if(bytes.IsNull())
        throw ArgumentNullException(L"bytes");
      if(charIndex < 0 || charIndex > (int)chars.Length())
        throw ArgumentOutOfRangeException(L"charIndex", L"Argument index is out of array range.");
      if(charCount < 0 || charCount > ((int)chars.Length() - charIndex))
        throw ArgumentOutOfRangeException(L"charCount", L"Argument index is out of array range.");
      if(byteIndex < 0 || byteIndex > (int)bytes.Length())
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
      if((int)bytes.Length() - byteIndex < charCount)

        if (charCount == 0)
          return 0;

      wstring cptr = chars.ToPtr();
      byte* bptr = bytes.ToPtr();
      return GetBytesImpl(cptr + charIndex, charCount, bptr + byteIndex, (int)bytes.Length() - byteIndex);
      }
    int CrapoEncoding::GetBytes(String& s, int charIndex, int charCount, ByteArray& bytes, int byteIndex)
      {
      // Validate the parameters.
      if(bytes.IsNull())
        throw ArgumentNullException(L"bytes");
      if(charIndex < 0 || charIndex > (int)s.Length())
        throw ArgumentOutOfRangeException(L"charIndex", L"String index is out of range.");
      if(charCount < 0 || charCount > ((int)s.Length() - charIndex))
        throw ArgumentOutOfRangeException(L"charCount", L"String length is out of range.");
      if(byteIndex < 0 || byteIndex > (int)bytes.Length())
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
      if(((int)bytes.Length() - byteIndex) < charCount)
        throw ArgumentException(L"Insufficient space in the argument array.", L"bytes");

      if (charCount == 0 || (int)bytes.Length() == byteIndex)
        return 0;
      cstring cptr = s;
      byte* bptr = bytes.ToPtr();
      return GetBytesImpl((wstring)cptr + charIndex, charCount, bptr + byteIndex, (int)bytes.Length() - byteIndex);
      }
    int CrapoEncoding::GetBytes(wchar_t* chars, int charCount, byte* bytes, int byteCount)
      {
      return GetBytesImpl(chars, charCount, bytes, byteCount);
      }
    ByteArray CrapoEncoding::GetBytes(String& s)
      {
      return Text::Encoding::GetBytes(s);
      }
    ByteArray CrapoEncoding::GetBytes(CharArray& chars, int index, int count)
      {
      return Text::Encoding::GetBytes(chars, index, count);
      }
    ByteArray CrapoEncoding::GetBytes(CharArray& chars)
      {
      return Text::Encoding::GetBytes(chars);
      }
    // ------------------------------------------------------------------------

    int CrapoEncoding::GetBytesInternal(wchar_t*, int, byte*, int, bool, Object*)
		{
			throw Exception(L"Statefull encoding is not implemented (yet?) by this encoding class.");
		}

    void CrapoEncoding::HandleFallback(Text::GCEncoderFallbackBuffer& buffer,wchar_t* chars, int& charIndex, int& charCount, byte* bytes, int& byteIndex, int& byteCount)
      {
      HandleFallback(buffer, chars, charIndex, charCount, bytes, byteIndex, byteCount, nullptr);
      }

    void CrapoEncoding::HandleFallback(Text::GCEncoderFallbackBuffer& buffer, wchar_t* chars, int& charIndex, int& charCount, byte* bytes, int& byteIndex, int& byteCount, Object* state)
      {
      if(buffer.Get() == nullptr)
        buffer.Set(_encoder_fallback->CreateFallbackBuffer());

      if (charCount > 1 && (Char::IsSurrogate (chars [charIndex]) && Char::IsSurrogate (chars [charIndex + 1]))) 
        {
        buffer->Fallback(chars[charIndex], chars[charIndex + 1], charIndex);
        charIndex++;
        charCount--;
        }
      else
        buffer->Fallback(chars[charIndex], charIndex);
      CharArray tmp(buffer->Remaining());
      int idx = 0;
      while (buffer->Remaining() > 0)
        tmp [idx++] = buffer->GetNextChar();

      wchar_t* tmparr = tmp.ToPtr(); 
      byte* outbytes = bytes == nullptr ? nullptr : bytes + byteIndex;
      int len = state == nullptr ? GetBytes(tmparr, (int)tmp.Length(), outbytes, byteCount)
        : GetBytesInternal(tmparr, (int)tmp.Length(), outbytes, byteCount, true, state);

      byteIndex += len;
      byteCount -= len;
      }

    int CrapoEncoding::WindowsCodePage()
      {
      return _win_code_page != 0 ? _win_code_page : Text::Encoding::WindowsCodePage();
      }
    }
  }
