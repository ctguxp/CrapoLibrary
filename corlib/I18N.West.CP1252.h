// Perch Lake Computer System

#pragma once
#include "I18N.Common.ByteEncoding.h"

namespace I18N
  {
  namespace West
    {
    class CP1252 : public Common::ByteEncoding
      {
      private:
        static wchar_t ToChars[256];
      public:
        CP1252();
        ~CP1252();
        virtual int GetByteCountImpl(wchar_t*, int) override;
        virtual int GetByteCount(String&) override;
        virtual int GetByteCount(CharArray&) override;
        virtual int GetByteCount(wchar_t*, int) override;
      
        int GetBytesImpl(wchar_t* chars, int charCount, byte* bytes, int byteCount) override;

      protected:
        virtual void ToBytes(wchar_t* chars, int charCount, byte* bytes, int byteCount) override;
      };
    }
  }
