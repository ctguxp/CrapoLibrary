// Perch Lake Computer System

#pragma once
#include "I18N.Common.CrapoEncoding.h"
#include "System.Array.h"
#include "System.String.h"

namespace I18N
  {
  namespace Common
    {
    class ByteEncoding : public CrapoEncoding
      {
      protected:
        CharArray _toChars;
        String    _encodingName;
	      String    _bodyName;
	      String    _headerName;
	      String    _webName;
	      bool      _isBrowserDisplay;
	      bool      _isBrowserSave;
	      bool      _isMailNewsDisplay;
	      bool      _isMailNewsSave;
	      int       _windowsCodePage;
      public:
        ByteEncoding(int, CharArray, String, String, String, String, bool, bool, bool, bool, int);
        ~ByteEncoding();

        virtual int GetByteCountImpl(wchar_t*, int) override;
        virtual int GetByteCount(String&) override;
        virtual int GetByteCount(CharArray&) override;
        virtual int GetByteCount(wchar_t*, int) override;

        virtual int GetBytesImpl(wchar_t* chars, int charCount, byte* bytes, int byteCount) override;

        virtual int GetCharCount(ByteArray&s, int, int) override;
        virtual int GetCharCount(ByteArray&) override;
        virtual int GetCharCount(byte*, int) override;

        virtual int GetChars(ByteArray&, int, int, CharArray&, int) override;
        virtual CharArray GetChars(ByteArray&) override;
        virtual CharArray GetChars(ByteArray&, int, int) override;
        virtual int GetChars(byte*, int, wchar_t*, int) override;

        virtual int GetMaxByteCount(int) override;
        virtual int GetMaxCharCount(int) override;

        virtual bool IsSingleByte();
      protected:
        virtual void ToBytes(wchar_t* chars, int charCount, byte* bytes, int byteCount) = 0;
      };
    }
  }
