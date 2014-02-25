#pragma once
#include "System.Text.Encoding.h"
#include "System.Array.hpp"

namespace System
  {
  namespace Text
    {
    class UnicodeEncoding : public Encoding
      {
      enum
        {
        UNICODE_CODE_PAGE = 1200,
        BIG_UNICODE_CODE_PAGE = 1201
        };
      public:
        // Default constructor
        UnicodeEncoding(bool bigEndian = false, bool byteOrderMark = true, bool throwOnInvalidBytes = false);
        // Destructor
        ~UnicodeEncoding();

        virtual int GetByteCount(CharArray&, int, int) override;
        virtual int GetByteCount(String&) override;
        virtual int GetByteCount(CharArray&) override;
        virtual int GetByteCount(wchar_t*, int) override;

        virtual int GetBytes(CharArray&, int, int, ByteArray&, int) override;
        virtual int GetBytes(String&, int, int, ByteArray&, int) override;
        virtual int GetBytes(wchar_t*, int, byte*, int) override;
        virtual ByteArray GetBytes(String&) override;
        virtual ByteArray GetBytes(CharArray&, int, int) override;
        virtual ByteArray GetBytes(CharArray&) override;

        virtual int GetCharCount(ByteArray&s, int, int) override;
        virtual int GetCharCount(ByteArray&) override;

        virtual int GetChars(ByteArray&, int, int, CharArray&, int) override;
        virtual CharArray GetChars(ByteArray&) override;
        virtual CharArray GetChars(ByteArray&, int, int) override;
        virtual int GetChars(byte*, int, wchar_t*, int) override;

        virtual int GetMaxByteCount(int) override;
        virtual int GetMaxCharCount(int) override;

        virtual ByteArray GetPreamble() override;
        virtual int GetCharCount (byte* bytes, int count) override;
      private:
        int GetCharsInternal(byte* bytes, int byteCount, string chars, int charCount);
        void CopyChars(byte* src, byte* dest, int count, bool bigEndian);
      private:
        bool _isThrowException;
        bool _bigEndian;
        bool _byteOrderMark;
      };
    }
  }