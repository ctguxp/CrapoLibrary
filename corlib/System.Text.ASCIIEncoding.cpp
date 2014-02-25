// Perch Lake Computer System

#include "pch.h"
#include "System.Text.ASCIIEncoding.h"
#include "System.h"
#include "System.Text.DecoderFallback.h"
#include "System.Text.EncoderFallback.h"

namespace System
  {
  namespace Text
    {
    // Default constructor
    ASCIIEncoding::ASCIIEncoding()
      :Encoding(ASCII_CODE_PAGE)
      {
      }
    // Destructor
    ASCIIEncoding::~ASCIIEncoding()
      {
      }

    bool ASCIIEncoding::IsSingleByte()
      {
      return true;
      }

    int ASCIIEncoding::GetByteCount(CharArray& chars, int index, int count)
      {
      if(chars.IsNull())
        throw ArgumentNullException(L"chars");
      if(index < 0 || index > (int)chars.Length())
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
      if(count < 0 || count > ((int)chars.Length() - index))
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");
      return count;
      }

    int ASCIIEncoding::GetByteCount(String& s)
      {
      return Encoding::GetByteCount(s);
      }

    int ASCIIEncoding::GetByteCount(CharArray& chars)
      {
      return Encoding::GetByteCount(chars);
      }

    int ASCIIEncoding::GetByteCount(wchar_t*, int count)
      {
      return count;
      }

    ByteArray ASCIIEncoding::GetBytes(String& s)
      {
      return Encoding::GetBytes(s);
      }

    ByteArray ASCIIEncoding::GetBytes(CharArray& chars)
      {
      return Encoding::GetBytes(chars);
      }

    int ASCIIEncoding::GetBytes(System::CharArray& chars, int charIndex, int charCount, System::ByteArray& bytes, int byteIndex)
      {
      System::CharArray fbchars;
      GCEncoderFallbackBuffer buffer;
      return InternalGetBytes(chars, charIndex, charCount, bytes, byteIndex, buffer, fbchars);
      }

    int ASCIIEncoding::GetBytes(System::String& chars, int charIndex, int charCount, System::ByteArray& bytes, int byteIndex)
      {
      System::CharArray fbchars;
      GCEncoderFallbackBuffer buffer;
      return InternalGetBytes(chars, charIndex, charCount, bytes, byteIndex, buffer, fbchars);
      }

    int ASCIIEncoding::InternalGetBytes(String& chars, int charIndex, int charCount, ByteArray bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray& fallback_chars)
      {
      /*if(chars.IsNull())
        throw ArgumentNullException(L"chars");*/

      cstring cptr = chars;
      return InternalGetBytes((string)cptr, chars.Length(), charIndex, charCount, bytes, byteIndex, buffer, fallback_chars);
      }

    ByteArray ASCIIEncoding::GetBytes(CharArray& chars, int index, int count)
      {
      return Encoding::GetBytes(chars, index, count);
      }

    int ASCIIEncoding::GetBytes(wchar_t *chars, int charCount, byte *bytes, int byteCount)
      {
      if(chars == nullptr)
        throw ArgumentNullException(L"chars");
      if(bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      if(charCount < 0)
        throw ArgumentOutOfRangeException(L"charCount");
      if(byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount");

      if (byteCount < charCount)
        throw ArgumentException(L"bytecount is less than the number of bytes required", L"byteCount");

      for(int i = 0; i < charCount; i++)
        {
        wchar_t c = chars [i];
        bytes [i] = (byte)((c < (wchar_t) 0x80) ? c : L'?');
        }
      return charCount;
      }

    int ASCIIEncoding::GetCharCount(ByteArray& bytes, int index, int count)
      {
      if(bytes.IsNull()) 
        throw ArgumentNullException(L"bytes");
      if(index < 0 || index > (int)bytes.Length()) 
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
      if(count < 0 || count > ((int)bytes.Length() - index)) 
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");
      return count;
      }

    int ASCIIEncoding::GetCharCount(ByteArray& bytes)
      {
      return Encoding::GetCharCount(bytes);
      }

    int ASCIIEncoding::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex)
      {
      GCDecoderFallbackBuffer buffer;
      return GetChars(bytes, byteIndex, byteCount, chars, charIndex, buffer);
      }

    int ASCIIEncoding::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex, GCDecoderFallbackBuffer& buffer)
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
      throw ArgumentException(L"Arg_InsufficientSpace");

    int count = byteCount;
    while(count-- > 0) 
      {
      wchar_t c = (char) bytes [byteIndex++];
      if(c < 0x0080)
        chars[charIndex++] = c;
      else
        {
        if(buffer.Get() == nullptr)
          buffer.Set(_decoder_fallback->CreateFallbackBuffer());
        ByteArray thisByte(1);
        thisByte[0] = bytes[byteIndex-1];
        buffer->Fallback(thisByte, 0);
        while(buffer->Remaining() > 0)
          {
          if(charIndex < (int)chars.Length())
            {
            chars [charIndex++] = buffer->GetNextChar();
            continue;
            }
          throw ArgumentException(L"The output char buffer is too small to contain the decoded characters.");
          }
        }
      }
    return byteCount;
    }

   CharArray ASCIIEncoding::GetChars(ByteArray& bytes)
    {
    return Encoding::GetChars(bytes);
    }

  CharArray ASCIIEncoding::GetChars(ByteArray& bytes, int index, int count)
    {
    return Encoding::GetChars(bytes, index, count);
    }

  int ASCIIEncoding::GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount)
    {
    if(bytes == nullptr)
      throw ArgumentNullException(L"bytes");
    if (chars == nullptr) 
      throw ArgumentNullException(L"chars");
    if (charCount < 0)
      throw ArgumentOutOfRangeException(L"charCount");
    if (byteCount < 0)
      throw ArgumentOutOfRangeException(L"byteCount");
    if (charCount < byteCount)
      throw ArgumentException(L"charcount is less than the number of bytes required", L"charCount");

    for(int i = 0; i < byteCount; i++)
      {
      byte b = bytes[i];
      chars[i] = b > 127 ? '?' : (wchar_t)b;
      }
    return byteCount;
    }

  int ASCIIEncoding::GetCharCount(byte*, int count)
    {
    return count;
    }  

  int ASCIIEncoding::InternalGetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray& fallback_chars)
    {
    if(chars.IsNull())
      throw ArgumentNullException(L"chars");

    wchar_t *cptr = chars.ToPtr();
    return InternalGetBytes(cptr, (int)chars.Length(), charIndex, charCount, bytes, byteIndex, buffer, fallback_chars);
    }

  int ASCIIEncoding::InternalGetBytes(wchar_t *chars, int charLength, int charIndex, int charCount, ByteArray& bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray fallback_chars)
    {
    if(bytes.IsNull())
      throw ArgumentNullException(L"bytes");
    if(charIndex < 0 || charIndex > charLength)
      throw ArgumentOutOfRangeException(L"charIndex", L"ArgRange_StringIndex");
    if(charCount < 0 || charCount > (charLength - charIndex))
      throw ArgumentOutOfRangeException(L"charCount", L"ArgRange_StringRange");
    if (byteIndex < 0 || byteIndex > (int)bytes.Length())
      throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
    if (((int)bytes.Length() - byteIndex) < charCount)
      throw ArgumentException (L"Arg_InsufficientSpace");

    int count = charCount;
    wchar_t ch;
    while (count-- > 0) 
      {
      ch = chars [charIndex++];
      if (ch < (wchar_t)0x80) 
        {
        bytes [byteIndex++] = (byte)ch;
        } 
      else
        {
        if(buffer.Get() == nullptr)
          buffer.Set(_encoder_fallback->CreateFallbackBuffer());
        if(Char::IsSurrogate (ch) && count > 1 &&
          Char::IsSurrogate(chars [charIndex]))
          buffer->Fallback (ch, chars [charIndex], charIndex++ - 1);
        else
          buffer->Fallback(ch, charIndex - 1);
        if(fallback_chars.IsNull() || (int)fallback_chars.Length() < buffer->Remaining())
          {
          fallback_chars.Length(0);
          fallback_chars.Length(buffer->Remaining());
          }
        for(uint32 i = 0; i < fallback_chars.Length(); i++)
          fallback_chars [i] = buffer->GetNextChar();
        byteIndex += InternalGetBytes(fallback_chars, 0, (int)fallback_chars.Length(), bytes, byteIndex, buffer, fallback_chars);
        }
      }
    return charCount;
    }


  int ASCIIEncoding::GetMaxByteCount(int charCount)
    {
    if (charCount < 0)
      throw System::Exception(L"Argument out of range");
    return charCount;
    }
  int ASCIIEncoding::GetMaxCharCount(int byteCount)
    {
    if (byteCount < 0)
      throw System::Exception(L"Argument out of range");
    return byteCount;
    }
  }
  }