// Perch Lake Computer System

#pragma once
#include "System.Text.Encoding.h"
#include "System.Text.EncoderFallbackBuffer.h"
#include "System.Text.DecoderFallbackBuffer.h"

namespace System
  {
  namespace Text
    {
    class CRAPOCOREDLL_API ASCIIEncoding : public Encoding
      {
      private:
        static const int ASCII_CODE_PAGE = 20127;
      public:
        ASCIIEncoding();
        ~ASCIIEncoding();

        virtual bool IsSingleByte() override;
      
        virtual int GetByteCount(CharArray&, int, int) override;
        virtual int GetByteCount(wchar_t*, int) override;
        virtual int GetByteCount(String&) override;
        virtual int GetByteCount(CharArray& chars) override;

        virtual ByteArray GetBytes(String&) override;
        virtual ByteArray GetBytes(CharArray& chars) override;
        virtual int GetBytes(CharArray&, int, int, ByteArray&, int) override;
        virtual int GetBytes(String&, int, int, ByteArray&, int) override;
        virtual int GetBytes(wchar_t*, int, byte*, int) override;
        virtual ByteArray GetBytes(CharArray& chars, int index, int count) override;

        virtual int GetCharCount(ByteArray& bytes, int index, int count) override;
        virtual int GetCharCount(ByteArray& bytes) override;
        virtual int GetCharCount(byte *bytes, int count) override;

        virtual int GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex) override;
        virtual CharArray GetChars(ByteArray&) override;
        virtual CharArray GetChars(ByteArray& bytes, int index, int count) override;
        virtual int GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount) override;       
       
        virtual int GetMaxByteCount(int) override;
        virtual int GetMaxCharCount(int) override;
      protected:
        int InternalGetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray& fallback_chars);
        int InternalGetBytes(String& chars, int charIndex, int charCount, ByteArray bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray& fallback_chars);
        int InternalGetBytes(wchar_t *chars, int charLength, int charIndex, int charCount, ByteArray& bytes, int byteIndex, GCEncoderFallbackBuffer& buffer, CharArray fallback_chars);
      private:
        int GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex, GCDecoderFallbackBuffer& buffer);

      friend Encoding;
      };
    }
  }
