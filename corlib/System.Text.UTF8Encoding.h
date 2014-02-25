// Perch Lake Computer System

#pragma once
#include "System.Text.Encoding.h"
#include "System.Text.EncoderFallbackBuffer.h"
#include "System.Text.DecoderFallbackBuffer.h"

namespace System
  {
  namespace Text
    {
    class CRAPOCOREDLL_API UTF8Encoding : public Encoding
      {
      private:
        static const int UTF8_CODE_PAGE = 65001;
        bool _isThrowException;
        bool _emitUTF8Identifier;
      public:
        UTF8Encoding(bool encoderShouldEmitUTF8Idenditfier = false, bool throwOnInvalidBytes = false);
        ~UTF8Encoding();

        virtual int GetByteCount(CharArray& chars, int index, int count) override;
        virtual int GetByteCount(wchar_t* chars, int count) override;
        virtual int GetByteCount(CharArray& chars) override;
        virtual int GetByteCount(String&) override;

        virtual ByteArray GetBytes(String&) override;
        virtual ByteArray GetBytes(CharArray& chars) override;
        virtual int GetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex) override;        
        virtual int GetBytes(String&, int, int, ByteArray&, int) override;
        virtual int GetBytes(wchar_t*, int, byte*, int) override;
        virtual ByteArray GetBytes(CharArray& chars, int index, int count) override;

        virtual int GetCharCount(ByteArray& bytes, int index, int count) override;
        virtual int GetCharCount(ByteArray& bytes) override;
        virtual int GetCharCount(byte* bytes, int count) override;

        virtual int GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex) override;
        virtual CharArray GetChars(ByteArray&) override;
        virtual CharArray GetChars(ByteArray& bytes, int index, int count) override;
        virtual int GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount) override;

        virtual ByteArray GetPreamble() override;
        virtual int GetMaxCharCount(int) override;
        virtual int GetMaxByteCount(int) override;
        
      private:
        static CharArray GetFallbackChars(const wchar_t* chars, const wchar_t* start, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer);
        static int InternalGetByteCount(CharArray& chars, int index, int count, EncoderFallback* fallback, wchar_t& leftOver, bool flush);
        static int InternalGetByteCount(wchar_t* chars, int count, EncoderFallback* fallback, wchar_t& leftOver, bool flush);
        static int InternalGetBytes(CharArray& chars, int charIndex, int charCount, ByteArray& bytes, int byteIndex, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer, wchar_t& leftOver, bool flush);
        static int InternalGetBytes(const wchar_t* chars, int count, byte* bytes, int bcount, EncoderFallback* fallback, GCEncoderFallbackBuffer& buffer, wchar_t leftOver, bool flush);
        static int InternalGetCharCount(ByteArray& bytes, int index, int count, uint32 leftOverBits, uint32 leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush);
        static int InternalGetCharCount(byte* bytes, int count, uint32 leftOverBits, uint32 leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush);
        static int InternalGetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex, uint32& leftOverBits, uint32& leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush);
        static int InternalGetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount, uint32& leftOverBits, uint32& leftOverCount, DecoderFallback* fallback, GCDecoderFallbackBuffer& fallbackBuffer, ByteArray& bufferArg, bool flush);
        static int Fallback(DecoderFallback* fallback, GCDecoderFallbackBuffer& buffer, ByteArray& bufferArg, byte* bytes, long index, uint32 size);
        static void Fallback(DecoderFallback* fallback, GCDecoderFallbackBuffer& buffer, ByteArray& bufferArg, byte* bytes, long byteIndex, uint32 size, wchar_t* chars, int& charIndex);

      friend Encoding;
      };
    }
  }
