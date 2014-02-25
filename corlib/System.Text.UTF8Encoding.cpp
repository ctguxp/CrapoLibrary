// Perch Lake Computer System

#include "pch.h"
#include "System.Text.UTF8Encoding.h"
#include "System.Exception.h"
#include "System.Array.hpp"
#include "System.Text.EncoderFallback.h"
#include "System.Text.DecoderFallback.h"

namespace System
  {
  namespace Text
    {
    // Default constructor
    UTF8Encoding::UTF8Encoding(bool encoderShouldEmitUTF8Idenditfier, bool throwOnInvalidBytes)
      :Encoding(UTF8_CODE_PAGE)
      ,_isThrowException(throwOnInvalidBytes)
      ,_emitUTF8Identifier(encoderShouldEmitUTF8Idenditfier)
      {
      }
    // Destructor
    UTF8Encoding::~UTF8Encoding()
      {
      }

#pragma warning(disable:4100)

    int UTF8Encoding::GetByteCount(CharArray& chars, int index, int count)
      {
      throw System::ArgumentException(L"Not Implemented", L"UTF8Encoding::GetByteCount");
      }

    int UTF8Encoding::GetByteCount(wchar_t* chars, int count)
      {
      if(chars == nullptr)
        throw ArgumentNullException(L"chars");
      if(count == 0)
        return 0;
      wchar_t dummy = L'\0';
      return InternalGetByteCount(chars, count, _encoder_fallback, dummy, true);
      }

    int UTF8Encoding::GetByteCount(String& s)
      {
      return Encoding::GetByteCount(s);
      }

    int UTF8Encoding::GetByteCount(CharArray& chars)
      {
      return Encoding::GetByteCount(chars);
      }

    //Returns a Unicode byte order mark encoded in UTF-8 format, if the constructor for this instance requests a byte order mark. 
    ByteArray UTF8Encoding::GetPreamble()
      {
      if(_emitUTF8Identifier)
        {
        System::ByteArray bom(3);
        bom[0] = 0xEF;
        bom[1] = 0xBB;
        bom[2] = 0xBF;
        return bom;
        }
      return System::ByteArray();
      }

    ByteArray UTF8Encoding::GetBytes(String& s)
      {
      return Encoding::GetBytes(s);
      }

    ByteArray UTF8Encoding::GetBytes(CharArray& chars)
      {
      return Encoding::GetBytes(chars);
      }

    int UTF8Encoding::GetBytes(String& s, int charIndex, int charCount, ByteArray& bytes, int byteIndex)
      {
      if(bytes.IsNull())
        {
        throw ArgumentNullException(L"bytes");
        }
      if (charIndex < 0 || charIndex > (int)s.Length()) 
        {
        throw ArgumentOutOfRangeException(L"charIndex", L"ArgRange_StringIndex");
        }
      if (charCount < 0 || charCount > ((int)s.Length() - charIndex)) 
        {
        throw ArgumentOutOfRangeException(L"charCount", L"ArgRange_StringRange");
        }
      if (byteIndex < 0 || byteIndex > (int)bytes.Length()) 
        {
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
        }

      if(charIndex == (int)s.Length())
        return 0;

      const wchar_t* cptr = s;
      wchar_t dummy = L'\0';
      GCEncoderFallbackBuffer buffer;
      if((int)bytes.Length() == byteIndex)
        return InternalGetBytes(cptr + charIndex, charCount, nullptr, 0, _encoder_fallback, buffer, dummy, true);
      byte* bptr = bytes.ToPtr();
      return InternalGetBytes(cptr + charIndex, charCount, bptr + byteIndex, (int)bytes.Length() - byteIndex, _encoder_fallback, buffer, dummy, true);
      }

    int UTF8Encoding::GetBytes(System::CharArray& chars, int charIndex, int charCount, System::ByteArray& bytes, int byteIndex)
      {
      wchar_t leftOver = L'\0';
      GCEncoderFallbackBuffer buffer;
      return InternalGetBytes(chars, charIndex, charCount, bytes, byteIndex, _encoder_fallback, buffer, leftOver, true);
      }

    ByteArray UTF8Encoding::GetBytes(CharArray& chars, int index, int count)
      {
      return Encoding::GetBytes(chars, index, count);
      }

    int UTF8Encoding::InternalGetByteCount(CharArray& chars, int index, int count, EncoderFallback* fallback, wchar_t& leftOver, bool flush)
      {
      // Validate the parameters.
      if (chars.IsNull())
        {
        throw ArgumentNullException(L"chars");
        }
      if (index < 0 || index > (int)chars.Length()) 
        {
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
        }
      if (count < 0 || count > ((int)chars.Length() - index)) 
        {
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");
        }

      if (index == (int)chars.Length()) 
        {
        if (flush && leftOver != L'\0') 
          {
          // Flush the left-over surrogate pair start.
          leftOver = L'\0';
          return 3;
          }
        return 0;
        }

      string cptr = chars.ToPtr(); 
      return InternalGetByteCount(cptr + index, count, fallback, leftOver, flush);
      }

    int UTF8Encoding::InternalGetByteCount(wchar_t* chars, int count, EncoderFallback* fallback, wchar_t& leftOver, bool flush)
      {
      int length = 0;
      wchar_t* end = chars + count;
      wchar_t* start = chars;
      while(chars < end) 
        {
        if(leftOver == 0) {
          for (; chars < end; chars++) 
            {
            if(*chars < 0x0080) 
              {
              ++length;
              } 
            else if (*chars < 0x0800) 
              {
              length += 2;
              } 
            else if(*chars < 0xD800 || *chars > 0xDFFF) 
              {
              length += 3;
              } 
            else if(*chars <= 0xDBFF) 
              {
              // This is a surrogate start wchar_t, exit the inner loop only
              // if we don't find the complete surrogate pair.
              if (chars + 1 < end && chars [1] >= 0xDC00 && chars [1] <= 0xDFFF) 
                {
                length += 4;
                chars++;
                continue;
                }
              leftOver = *chars;
              chars++;
              break;
              } 
            else 
              {
              // We have a surrogate tail without 
              // leading surrogate.
              GCEncoderFallbackBuffer buffer;
              CharArray fallback_chars = GetFallbackChars(chars, start, fallback, buffer);
              wchar_t *fb_chars = fallback_chars.ToPtr();
              wchar_t dummy = L'\0';
              length += InternalGetByteCount(fb_chars, (int)fallback_chars.Length(), fallback, dummy, true);
              leftOver = L'\0';
              }
            }
          } 
        else 
          {
          if(*chars >= 0xDC00 && *chars <= 0xDFFF) 
            {
            // We have a correct surrogate pair.
            length += 4;
            chars++;
            } 
          else 
            {
            // We have a surrogate start followed by a
            // regular character.  Technically, this is
            // invalid, but we have to do something.
            // We write out the surrogate start and then
            // re-visit the current character again.
            GCEncoderFallbackBuffer buffer;
            CharArray fallback_chars = GetFallbackChars (chars, start, fallback, buffer);
            wchar_t *fb_chars = fallback_chars.ToPtr(); 
            wchar_t dummy = '\0';
            length += InternalGetByteCount(fb_chars, (int)fallback_chars.Length(), fallback, dummy, true);
            }
          leftOver = L'\0';
          }
        }
      if(flush) 
        {
        // Flush the left-over surrogate pair start.
        if (leftOver != L'\0')
          {
          length += 3;
          leftOver = L'\0';
          }
        }
      return length;
      }

    CharArray UTF8Encoding::GetFallbackChars(const wchar_t* chars, const wchar_t* start, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer)
      {
      if(buffer.Get() == nullptr)
        buffer.Set(fallback->CreateFallbackBuffer());

      buffer->Fallback(*chars, (int) (chars - start));

      CharArray fallback_chars(buffer->Remaining());
      for(uint32 i = 0; i < fallback_chars.Length(); i++)
        fallback_chars[i] = buffer->GetNextChar ();

      buffer->Reset();

      return fallback_chars;
      }

    int UTF8Encoding::GetBytes(wchar_t* chars, int charCount, byte* bytes, int byteCount)
      {
      if (chars == nullptr)
        throw ArgumentNullException(L"chars");
      // TODO: if (charCount < 0)
      //throw IndexOutOfRangeException(L"charCount");
      if (bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      // TODO: if (byteCount < 0)
      //throw IndexOutOfRangeException(L"charCount");

      if (charCount == 0)
        return 0;

      wchar_t dummy = L'\0';
      GCEncoderFallbackBuffer buffer;
      if (byteCount == 0)
        return InternalGetBytes(chars, charCount, nullptr, 0, _encoder_fallback, buffer, dummy, true);
      else
        return InternalGetBytes(chars, charCount, bytes, byteCount, _encoder_fallback, buffer, dummy, true);
      }

    int UTF8Encoding::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex)
      {
      uint32 leftOverBits = 0;
      uint32 leftOverCount = 0;
      GCDecoderFallbackBuffer buf;
      ByteArray bufferArg;
      return InternalGetChars(bytes, byteIndex, byteCount, chars,  charIndex, leftOverBits, leftOverCount, _decoder_fallback, buf, bufferArg, true);
      }

    CharArray UTF8Encoding::GetChars(ByteArray& bytes)
      {
      return Encoding::GetChars(bytes);
      }

    CharArray UTF8Encoding::GetChars(ByteArray& bytes, int index, int count)
      {
      return Encoding::GetChars(bytes, index, count);
      }

    int UTF8Encoding::GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount)
      {
      GCDecoderFallbackBuffer buf;
      ByteArray bufferArg;
      uint32 leftOverBits = 0;
      uint32 leftOverCount = 0;
      return InternalGetChars(bytes, byteCount, chars, charCount, leftOverBits, leftOverCount, _decoder_fallback,  buf, bufferArg, true);
      }

    int UTF8Encoding::InternalGetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex, uint32& leftOverBits, uint32& leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush)
      {
      // Validate the parameters.
      if(bytes.IsNull()) 
        {
        throw ArgumentNullException(L"bytes");
        }
      if(chars.IsNull()) 
        {
        throw ArgumentNullException(L"chars");
        }
      if(byteIndex < 0 || byteIndex > (int)bytes.Length()) 
        {
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
        }
      if(byteCount < 0 || byteCount > ((int)bytes.Length() - byteIndex)) 
        {
        throw ArgumentOutOfRangeException(L"byteCount", L"Argument index is out of array range.");
        }
      if(charIndex < 0 || charIndex > (int)chars.Length()) 
        {
        throw ArgumentOutOfRangeException(L"charIndex", L"Argument index is out of array range.");
        }

      if(charIndex == (int)chars.Length() && byteCount == 0)
        return 0;

      wchar_t* cptr = chars.ToPtr();
      if (byteCount == 0 || byteIndex == (int)bytes.Length())
        return InternalGetChars(nullptr, 0, cptr + charIndex, (int)chars.Length() - charIndex, leftOverBits, leftOverCount, fallback, fallbackBuffer, bufferArg, flush);
      // otherwise...
      byte* bptr = bytes.ToPtr();
      return InternalGetChars(bptr + byteIndex, byteCount, cptr + charIndex, (int)chars.Length() - charIndex, leftOverBits, leftOverCount, fallback, fallbackBuffer, bufferArg, flush);
      }

    int UTF8Encoding::InternalGetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount, uint32& leftOverBits, uint32& leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush)
      {
      int charIndex = 0, byteIndex = 0;
      int length = charCount;
      int posn = charIndex;

      if(leftOverCount == 0) 
        {
        int end = byteIndex + byteCount;
        for (; byteIndex < end; posn++, byteIndex++, byteCount--) 
          {
          if (bytes[byteIndex] < 0x80)
            {
            if(posn >= length) {
              throw ArgumentException(L"Arg_InsufficientSpace", L"chars");
              }
            chars[posn] = (wchar_t)bytes[byteIndex];
            } 
          else 
            {
            break;
            }
          }
        }

      // Convert the bytes into the output buffer.
      uint32 ch;
      uint32 leftBits = leftOverBits;
      uint32 leftSoFar = (leftOverCount & (uint32)0x0F);
      uint32 leftSize = ((leftOverCount >> 4) & (uint32)0x0F);

      int byteEnd = byteIndex + byteCount;
      for(; byteIndex < byteEnd; byteIndex++) 
        {
        // Fetch the next character from the byte buffer.
        ch = (uint32)(bytes[byteIndex]);
        if (leftSize == 0) 
          {
          // Process a UTF-8 start character.
          if (ch < (uint32)0x0080) 
            {
            // Single-byte UTF-8 character.
            if (posn >= length) 
              {
              throw ArgumentException(L"Arg_InsufficientSpace", L"chars");
              }
            chars[posn++] = (wchar_t)ch;
            } 
          else if ((ch & (uint32)0xE0) == (uint32)0xC0) 
            {
            // Double-byte UTF-8 character.
            leftBits = (ch & (uint32)0x1F);
            leftSoFar = 1;
            leftSize = 2;
            } 
          else if ((ch & (uint32)0xF0) == (uint32)0xE0) 
            {
            // Three-byte UTF-8 character.
            leftBits = (ch & (uint32)0x0F);
            leftSoFar = 1;
            leftSize = 3;
            } 
          else if ((ch & (uint32)0xF8) == (uint32)0xF0) 
            {
            // Four-byte UTF-8 character.
            leftBits = (ch & (uint32)0x07);
            leftSoFar = 1;
            leftSize = 4;
            } 
          else if ((ch & (uint32)0xFC) == (uint32)0xF8)
            {
            // Five-byte UTF-8 character.
            leftBits = (ch & (uint32)0x03);
            leftSoFar = 1;
            leftSize = 5;
            }
          else if ((ch & (uint32)0xFE) == (uint32)0xFC) 
            {
            // Six-byte UTF-8 character.
            leftBits = (ch & (uint32)0x03);
            leftSoFar = 1;
            leftSize = 6;
            } 
          else 
            {
            // Invalid UTF-8 start character.
            Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex, 1, chars, posn);
            }
          } 
        else 
          {
          // Process an extra byte in a multi-byte sequence.
          if ((ch & (uint32)0xC0) == (uint32)0x80) 
            {
            leftBits = ((leftBits << 6) | (ch & (uint32)0x3F));
            if (++leftSoFar >= leftSize) 
              {
              // We have a complete character now.
              if (leftBits < (uint32)0x10000) 
                {
                // is it an overlong ?
                bool overlong = false;
                switch (leftSize) 
                  {
                  case 2:
                    overlong = (leftBits <= 0x7F);
                    break;
                  case 3:
                    overlong = (leftBits <= 0x07FF);
                    break;
                  case 4:
                    overlong = (leftBits <= 0xFFFF);
                    break;
                  case 5:
                    overlong = (leftBits <= 0x1FFFFF);
                    break;
                  case 6:
                    overlong = (leftBits <= 0x03FFFFFF);
                    break;
                  }
                if(overlong) 
                  {
                  Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex - leftSoFar, leftSoFar, chars, posn);
                  }
                else if ((leftBits & 0xF800) == 0xD800)
                  {
                  // UTF-8 doesn't use surrogate characters
                  Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex - leftSoFar, leftSoFar, chars, posn);
                  }
                else 
                  {
                  if (posn >= length) 
                    {
                    throw ArgumentException(L"Arg_InsufficientSpace", L"chars");
                    }
                  chars[posn++] = (wchar_t)leftBits;
                  }
                } 
              else if (leftBits < (uint32)0x110000) 
                {
                if ((posn + 2) > length) 
                  {
                  throw ArgumentException(L"Arg_InsufficientSpace", L"chars");
                  }
                leftBits -= (uint32)0x10000;
                chars[posn++] = (wchar_t)((leftBits >> 10) +
                  (uint32)0xD800);
                chars[posn++] =(wchar_t)((leftBits & (uint32)0x3FF) + (uint32)0xDC00);
                } 
              else 
                {
                Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex - leftSoFar, leftSoFar, chars, posn);
                }
              leftSize = 0;
              }
            } else {
              // Invalid UTF-8 sequence: clear and restart.
              Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex - leftSoFar, leftSoFar, chars, posn);
              leftSize = 0;
              --byteIndex;
            }
          }
        }
      if (flush && leftSize != 0)
        {
        // We had left-over bytes that didn't make up
        // a complete UTF-8 character sequence.
        Fallback(fallback, fallbackBuffer, bufferArg, bytes, byteIndex - leftSoFar, leftSoFar, chars, posn);
        }
      leftOverBits = leftBits;
      leftOverCount = (leftSoFar | (leftSize << 4));

      // Return the final length to the caller.
      return posn - charIndex;
      }

    int UTF8Encoding::InternalGetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer, wchar_t& leftOver, bool flush)
      {
      // Validate the parameters.
      if (chars.IsNull())
        {
        throw ArgumentNullException(L"chars");
        }
      if(bytes.IsNull()) 
        {
        throw ArgumentNullException(L"bytes");
        }
      if(charIndex < 0 || charIndex > (int)chars.Length()) 
        {
        throw ArgumentOutOfRangeException(L"charIndex", L"Argument index is out of array range.");
        }
      if(charCount < 0 || charCount > ((int)chars.Length() - charIndex)) 
        {
        throw ArgumentOutOfRangeException(L"charCount", L"Argument index is out of array range.");
        }
      if(byteIndex < 0 || byteIndex > (int)bytes.Length())
        {
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");
        }

      if(charIndex == (int)chars.Length()) 
        {
        if (flush && leftOver != '\0') 
          {
          // FIXME: use EncoderFallback.
          //
          // By default it is empty, so I do nothing for now.
          leftOver = L'\0';
          }
        return 0;
        }


      wchar_t* cptr = chars.ToPtr();
      if((int)bytes.Length() == byteIndex)
        return InternalGetBytes(cptr + charIndex, charCount, nullptr, 0, fallback, buffer, leftOver, flush);
      byte *bptr = bytes.ToPtr();
      return InternalGetBytes(cptr + charIndex, charCount, bptr + byteIndex, (int)bytes.Length() - byteIndex, fallback, buffer, leftOver, flush);
      }

    int UTF8Encoding::InternalGetBytes(const wchar_t* chars, int count, byte* bytes, int bcount, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer, wchar_t leftOver, bool flush)
      {
      const wchar_t* end = chars + count;
      const wchar_t* start = chars;
      byte* start_bytes = bytes;
      byte* end_bytes = bytes + bcount;
      while(chars < end) 
        {
        if(leftOver == 0) 
          {
          for (; chars < end; chars++) 
            {
            int ch = *chars;
            if (ch < 0x0080)
              {
              if (bytes >= end_bytes)
                throw ArgumentException(L"Insufficient Space", L"bytes");
              *bytes++ = (byte)ch;
              } 
            else if(ch < 0x0800) 
              {
              if (bytes + 1 >= end_bytes)
                throw ArgumentException(L"Insufficient Space", L"bytes");
              bytes [0] = (byte) (0xC0 | (ch >> 6));
              bytes [1] = (byte) (0x80 | (ch & 0x3F));
              bytes += 2;
              } 
            else if(ch < 0xD800 || ch > 0xDFFF) 
              {
              if (bytes + 2 >= end_bytes)
                throw ArgumentException(L"Insufficient Space", L"bytes");
              bytes [0] = (byte) (0xE0 | (ch >> 12));
              bytes [1] = (byte) (0x80 | ((ch >> 6) & 0x3F));
              bytes [2] = (byte) (0x80 | (ch & 0x3F));
              bytes += 3;
              } 
            else if(ch <= 0xDBFF) 
              {
              // This is a surrogate wchar_t, exit the inner loop.
              leftOver = *chars;
              chars++;
              break;
              } 
            else 
              {
              // We have a surrogate tail without 
              // leading surrogate.
              CharArray fallback_chars = GetFallbackChars(chars, start, fallback, buffer); 
              wchar_t dummy = L'\0';
              if(bytes + InternalGetByteCount(fallback_chars, 0, (int)fallback_chars.Length(), fallback, dummy, true) > end_bytes)
                throw ArgumentException(L"Insufficient Space", L"bytes");
              wchar_t* fb_chars = fallback_chars.ToPtr();
              bytes += InternalGetBytes (fb_chars, (int)fallback_chars.Length(), bytes, bcount - (int) (bytes - start_bytes), fallback, buffer, dummy, true);
              leftOver = '\0';
              }
            }
          } 
        else
          {
          if (*chars >= 0xDC00 && *chars <= 0xDFFF) 
            {
            // We have a correct surrogate pair.
            int ch = 0x10000 + (int) *chars - 0xDC00 + (((int) leftOver - 0xD800) << 10);
            if (bytes + 3 >= end_bytes)
              throw ArgumentException(L"Insufficient Space", L"bytes");
            bytes [0] = (byte) (0xF0 | (ch >> 18));
            bytes [1] = (byte) (0x80 | ((ch >> 12) & 0x3F));
            bytes [2] = (byte) (0x80 | ((ch >> 6) & 0x3F));
            bytes [3] = (byte) (0x80 | (ch & 0x3F));
            bytes += 4;
            chars++;
            } 
          else 
            {
            // We have a surrogate start followed by a
            // regular character.  Technically, this is
            // invalid, but we have to do something.
            // We write out the surrogate start and then
            // re-visit the current character again.
            CharArray fallback_chars = GetFallbackChars (chars, start, fallback, buffer); 
            wchar_t dummy = '\0';
            if (bytes + InternalGetByteCount(fallback_chars, 0, (int)fallback_chars.Length(), fallback, dummy, true) > end_bytes)
              throw ArgumentException(L"Insufficient Space", L"bytes");
            wchar_t *fb_chars = fallback_chars.ToPtr();
            InternalGetBytes (fb_chars, (int)fallback_chars.Length(), bytes, bcount - (int) (bytes - start_bytes), fallback, buffer, dummy, true);
            leftOver = L'\0';
            }
          leftOver = L'\0';
          }
        }
      if (flush) 
        {
        // Flush the left-over surrogate pair start.
        if (leftOver != '\0') 
          {
          int ch = leftOver;
          if (bytes + 2 < end_bytes) 
            {
            bytes [0] = (byte) (0xE0 | (ch >> 12));
            bytes [1] = (byte) (0x80 | ((ch >> 6) & 0x3F));
            bytes [2] = (byte) (0x80 | (ch & 0x3F));
            bytes += 3;
            } 
          else 
            {
            throw ArgumentException(L"Insufficient Space", L"bytes");
            }
          leftOver = '\0';
          }
        }
      return (int)(bytes - (end_bytes - bcount));
      }    


    int UTF8Encoding::GetMaxCharCount(int byteCount)
      {
      if(byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount", L"ArgRange_NonNegative");

      // Add 1 to byteCount since there may be the bytes from part of a surrogate pair left from the previous call to GetChars/Decoder.Convert
      int maxCharCount = byteCount + 1;
      if(_decoder_fallback->MaxCharCount() > 1)
        maxCharCount = maxCharCount * _decoder_fallback->MaxCharCount();

      return maxCharCount;
      }

    int UTF8Encoding::GetMaxByteCount(int charCount)
      {
      if(charCount < 0)
        {
        throw ArgumentOutOfRangeException(L"charCount", L"ArgRange_NonNegative");
        }

      // Add 1 to charCount since there may be a lead surrogate left from the previous call to GetBytes/Encoder.Convert
      charCount = charCount + 1;
      if(_encoder_fallback->MaxCharCount() > 1)
        {
        charCount = charCount * _encoder_fallback->MaxCharCount();
        }

      return charCount * 3;
      }

    int UTF8Encoding::GetCharCount(ByteArray& bytes, int index, int count)
      {
      GCDecoderFallbackBuffer buf;
      ByteArray bufferArg;
      return InternalGetCharCount(bytes, index, count, 0, 0, _decoder_fallback, buf, bufferArg, true);
      }

    int UTF8Encoding::GetCharCount(ByteArray& bytes)
      {
      return Encoding::GetCharCount(bytes);
      }

    int UTF8Encoding::GetCharCount (byte* bytes, int count)
      {
      GCDecoderFallbackBuffer buf;
      ByteArray bufferArg;
      return InternalGetCharCount(bytes, count, 0, 0, _decoder_fallback, buf, bufferArg, true);
      }

    int UTF8Encoding::InternalGetCharCount(ByteArray& bytes, int index, int count, uint32 leftOverBits, uint32 leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush)
      {
      // Validate the parameters.
      if(bytes.IsNull()) 
        {
        throw ArgumentNullException(L"bytes");
        }
      if(index < 0 || index > (int)bytes.Length())
        {
        throw ArgumentOutOfRangeException(L"index", L"Argument index is out of array range.");
        }
      if(count < 0 || count > ((int)bytes.Length() - index)) 
        {
        throw ArgumentOutOfRangeException(L"count", L"Argument index is out of array range.");
        }

      if(count == 0)
        return 0;
      byte* bptr = bytes.ToPtr();
      return InternalGetCharCount(bptr + index, count, leftOverBits, leftOverCount, fallback, fallbackBuffer, bufferArg, flush);
      }

    int UTF8Encoding::InternalGetCharCount(byte* bytes, int count, uint32 leftOverBits, uint32 leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush)
      {
      int index = 0;

      int length = 0;

      if (leftOverCount == 0)
        {
        int end = index + count;
        for (; index < end; index++, count--) 
          {
          if (bytes [index] < 0x80)
            length++;
          else
            break;
          }
        }

      // Determine the number of characters that we have.
      uint32 ch;
      uint32 leftBits = leftOverBits;
      uint32 leftSoFar = (leftOverCount & (uint32)0x0F);
      uint32 leftSize = ((leftOverCount >> 4) & (uint32)0x0F);
      while (count > 0) 
        {
        ch = (uint32)(bytes[index++]);
        --count;
        if (leftSize == 0) 
          {
          // Process a UTF-8 start character.
          if (ch < (uint32)0x0080) 
            {
            // Single-byte UTF-8 character.
            ++length;
            } 
          else if ((ch & (uint32)0xE0) == (uint32)0xC0)
            {
            // Double-byte UTF-8 character.
            leftBits = (ch & (uint32)0x1F);
            leftSoFar = 1;
            leftSize = 2;
            } 
          else if ((ch & (uint32)0xF0) == (uint32)0xE0) 
            {
            // Three-byte UTF-8 character.
            leftBits = (ch & (uint32)0x0F);
            leftSoFar = 1;
            leftSize = 3;
            } 
          else if ((ch & (uint32)0xF8) == (uint32)0xF0) 
            {
            // Four-byte UTF-8 character.
            leftBits = (ch & (uint32)0x07);
            leftSoFar = 1;
            leftSize = 4;
            } 
          else if ((ch & (uint32)0xFC) == (uint32)0xF8)
            {
            // Five-byte UTF-8 character.
            leftBits = (ch & (uint32)0x03);
            leftSoFar = 1;
            leftSize = 5;
            } 
          else if ((ch & (uint32)0xFE) == (uint32)0xFC) 
            {
            // Six-byte UTF-8 character.
            leftBits = (ch & (uint32)0x03);
            leftSoFar = 1;
            leftSize = 6;
            } 
          else 
            {
            // Invalid UTF-8 start character.
            length += Fallback(fallback, fallbackBuffer, bufferArg, bytes, index - 1, 1);
            }
          } 
        else 
          {
          // Process an extra byte in a multi-byte sequence.
          if ((ch & (uint32)0xC0) == (uint32)0x80) 
            {
            leftBits = ((leftBits << 6) | (ch & (uint32)0x3F));
            if (++leftSoFar >= leftSize) 
              {
              // We have a complete character now.
              if (leftBits < (uint32)0x10000) 
                {
                // is it an overlong ?
                bool overlong = false;
                switch (leftSize) 
                  {
                  case 2:
                    overlong = (leftBits <= 0x7F);
                    break;
                  case 3:
                    overlong = (leftBits <= 0x07FF);
                    break;
                  case 4:
                    overlong = (leftBits <= 0xFFFF);
                    break;
                  case 5:
                    overlong = (leftBits <= 0x1FFFFF);
                    break;
                  case 6:
                    overlong = (leftBits <= 0x03FFFFFF);
                    break;
                  }
                if (overlong) 
                  {
                  length += Fallback(fallback, fallbackBuffer, bufferArg, bytes, index - leftSoFar, leftSoFar);
                  }
                else if ((leftBits & 0xF800) == 0xD800)
                  {
                  // UTF-8 doesn't use surrogate characters
                  length += Fallback(fallback, fallbackBuffer, bufferArg, bytes, index - leftSoFar, leftSoFar);
                  }
                else
                  ++length;
                } 
              else if (leftBits < (uint32)0x110000)
                {
                length += 2;
                } 
              else
                {
                Fallback(fallback, fallbackBuffer, bufferArg, bytes, index - leftSoFar, leftSoFar);
                }
              leftSize = 0;
              }
            } 
          else 
            {
            // Invalid UTF-8 sequence: clear and restart.
            length += Fallback(fallback, fallbackBuffer, bufferArg, bytes, index - leftSoFar, leftSoFar);
            leftSize = 0;
            --index;
            ++count;
            }
          }
        }
      if (flush && leftSize != 0) 
        {
        // We had left-over bytes that didn't make up
        // a complete UTF-8 character sequence.
        length += Fallback (fallback, fallbackBuffer, bufferArg, bytes, index - leftSoFar, leftSoFar);
        }

      // Return the final length to the caller.
      return length;
      }

    int UTF8Encoding::Fallback(DecoderFallback* fallback, GCDecoderFallbackBuffer& buffer, ByteArray& bufferArg, byte* bytes, long index, uint32 size)
      {
      if(buffer.Get() == nullptr)
        {
        if(fallback != nullptr)
          buffer.Set(fallback->CreateFallbackBuffer());
        // TODO: else
        //buffer = ((Decoder) provider).FallbackBuffer;
        }
      if(bufferArg.IsNull())
        bufferArg.Length(1);
      int ret = 0;
      for(uint32 i = 0; i < size; i++)
        {
        bufferArg[0] = bytes[(int) index + i];
        buffer->Fallback(bufferArg, 0);
        ret += buffer->Remaining();
        buffer->Reset();
        }
      return ret;
      }

    // Get Chars
    void UTF8Encoding::Fallback(DecoderFallback* fallback, GCDecoderFallbackBuffer& buffer, ByteArray& bufferArg, byte* bytes, long byteIndex, uint32 size, wchar_t* chars, int& charIndex)
      {
      if(buffer.Get() == nullptr) 
        {
        if(fallback != nullptr)
          buffer.Set(fallback->CreateFallbackBuffer());
        // TODO: else
        //buffer = ((Decoder) provider).FallbackBuffer;
        }
      if(bufferArg.IsNull())
        bufferArg.Length(1);
      for(uint32 i = 0; i < size; i++)
        {
        bufferArg[0] = bytes[byteIndex + i];
        buffer->Fallback(bufferArg, 0);
        while(buffer->Remaining() > 0)
          chars[charIndex++] = buffer->GetNextChar();
        buffer->Reset();
        }
      }

#pragma warning(default:4100)
    }
  }