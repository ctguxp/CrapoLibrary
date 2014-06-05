// Perch Lake Computer System

#pragma once
#include "System.Text.Encoding.h"
#include "System.Text.EncoderFallbackBuffer.h"

using namespace System;

namespace I18N
  {
  namespace Common
    {
    class CRAPOCOREDLL_API CrapoEncoding : public Text::Encoding
      {
      private:
        int _win_code_page;
      public:
        CrapoEncoding(int, int windowsCodePage = 0);
        ~CrapoEncoding();

        virtual int GetByteCountImpl(wchar_t*, int) = 0;
        virtual int GetByteCount(CharArray&, int, int) override;
        virtual int GetByteCount(String&) override;
        virtual int GetByteCount(CharArray&) override;
        virtual int GetByteCount(wchar_t*, int) override;

        virtual int GetBytesImpl(wchar_t*, int, byte*, int) = 0;
        virtual int GetBytes(CharArray&, int, int, ByteArray&, int) override;
        virtual int GetBytes(String&, int, int, ByteArray&, int);
        virtual int GetBytes(wchar_t*, int, byte*, int) override;
        virtual ByteArray GetBytes(String&) override;
        virtual ByteArray GetBytes(CharArray&, int, int) override;
        virtual ByteArray GetBytes(CharArray&) override;

        void HandleFallback(Text::GCEncoderFallbackBuffer& buffer,wchar_t* chars, int& charIndex, int& charCount, byte* bytes, int& byteIndex, int& byteCount);
        void HandleFallback(Text::GCEncoderFallbackBuffer& buffer, wchar_t* chars, int& charIndex, int& charCount, byte* bytes, int& byteIndex, int& byteCount, Object* state);
        
        virtual int WindowsCodePage() override;
      protected:
        virtual int GetBytesInternal(wchar_t *chars, int charCount, byte* bytes, int byteCount, bool flush, Object* state);
      };
    }
  }

