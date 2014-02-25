// Perch Lake Computer System

#include "pch.h"
#include "I18N.Common.ByteEncoding.h"
#include "System.Exception.h"
#include "System.Array.hpp"

namespace I18N
  {
  namespace Common
    {
    ByteEncoding::ByteEncoding(int codePage, CharArray toChars, String encodingName, String bodyName
      ,String headerName, String webName, bool isBrowserDisplay, bool isBrowserSave
      ,bool isMailNewsDisplay, bool isMailNewsSave, int windowsCodePage)
      :CrapoEncoding(codePage)
      ,_toChars(toChars)
      ,_encodingName(encodingName)
      ,_bodyName(bodyName)
      ,_headerName(headerName)
      ,_webName(webName)
      ,_isBrowserDisplay(isBrowserDisplay)
      ,_isBrowserSave(isBrowserSave)
      ,_isMailNewsDisplay(isMailNewsDisplay)
      ,_isMailNewsSave(isMailNewsSave)
      ,_windowsCodePage(windowsCodePage)
      {
      }
    ByteEncoding::~ByteEncoding()
      {
      }

    int ByteEncoding::GetByteCountImpl(wchar_t*, int count)
      {
      return count;
      }

    // ------------------------------------------------------------------------
    // GetByteCount function
    int ByteEncoding::GetByteCount(String& s)
      {
      return s.Length();
      }
    int ByteEncoding::GetByteCount(CharArray& chars)
      {
      return System::Text::Encoding::GetByteCount(chars);
      }
    int ByteEncoding::GetByteCount(wchar_t* chars, int count)
      {
      return System::Text::Encoding::GetByteCount(chars, count);
      }
    // ------------------------------------------------------------------------

    int ByteEncoding::GetBytesImpl(wchar_t* chars, int charCount, byte* bytes, int byteCount)
      {
      ToBytes(chars, charCount, bytes, byteCount);
      return charCount;
      }

    // ------------------------------------------------------------------------
    /// Get the number of characters needed to decode a byte buffer.
    int ByteEncoding::GetCharCount(ByteArray& bytes, int index, int count)
      {
      if(bytes.IsNull())
        throw ArgumentNullException(L"bytes");
      if(index < 0 || index > (int)bytes.Length())
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
      if(count < 0 || count > ((int)bytes.Length() - index))
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");
      return count;
      }
    int ByteEncoding::GetCharCount(ByteArray& bytes)
      {
      return Text::Encoding::GetCharCount(bytes);
      }
    int ByteEncoding::GetCharCount(byte* bytes, int count)
      {
      return Text::Encoding::GetCharCount(bytes, count);
      }
    // ------------------------------------------------------------------------

    int ByteEncoding::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex)
      {
      if(bytes.IsNull())
        throw ArgumentNullException(L"bytes");
      if(chars.IsNull())
        throw ArgumentNullException(L"chars");
      if(byteIndex < 0 || byteIndex > (int)bytes.Length())
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
      if(byteCount < 0 || byteCount > ((int)bytes.Length() - byteIndex))
        throw ArgumentOutOfRangeException(L"byteCount", L"Argument index is out of array range.");
      if(charIndex < 0 || charIndex > (int)chars.Length())
        throw ArgumentOutOfRangeException(L"charIndex", L"Argument index is out of array range.");
      if(((int)chars.Length() - charIndex) < byteCount)
        throw ArgumentException(L"Insufficient space in the argument array.");
      int count = byteCount;
      //char[] cvt = toChars;
      while(count-- > 0)
        {
        chars[charIndex++] = _toChars[(int)(bytes[byteIndex++])];
        }
      return byteCount;
      }
    CharArray ByteEncoding::GetChars(ByteArray& bytes, int index, int count)
      {
      return Text::Encoding::GetChars(bytes, index, count);
      }

    CharArray ByteEncoding::GetChars(ByteArray& bytes)
      {
      return Text::Encoding::GetChars(bytes);
      }

    int ByteEncoding::GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount)
      {
      return Text::Encoding::GetChars(bytes, byteCount, chars, charCount);
      }

    int ByteEncoding::GetMaxByteCount(int charCount)
      {
      if(charCount < 0)
        throw ArgumentOutOfRangeException(L"charCount", L"Non-negative value is expected.");
      return charCount;
      }
    int ByteEncoding::GetMaxCharCount(int byteCount)
      {
      if(byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount", L"Non-negative value is expected.");
      return byteCount;
      }

    bool ByteEncoding::IsSingleByte()
      {
      return true;
      }
    }
  }
