// Perch Lake Computer System

#pragma once
#include "Global.Memory.h"
#include "System.Object.h"
#include "System.Text.Decoder.h"
#include "System.Threading.CriticalSection.h"

#pragma warning(disable:4266)
namespace System
  {
  namespace Text
    {
    class Encoding;
    class DecoderFallback;
    class EncoderFallback;
    }
  namespace IO
    {
    class StreamReader;
    }
  namespace Text
    {
    typedef Global::SharedPtr<Encoding> GCEncoding;
    class CRAPOCOREDLL_API Encoding : public Object
      {
      private:
        class ForwardingDecoder : public Decoder
          {
          private:
            Encoding* _encoding;
          public:
            ForwardingDecoder(Encoding* enc);
            virtual int GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex) override;
          };
      protected:
        int  _codePage;
        int  _windows_code_page;  // Code page used by this encoding.
        bool _is_readonly;
        DecoderFallback* _decoder_fallback;
        EncoderFallback* _encoder_fallback;
      private:
#pragma warning(disable:4251)
        static GCEncoding _defaultEncoding;
        static GCEncoding _unicodeEncoding;
        static GCEncoding _bigEndianEncoding;
        static GCEncoding _utf8EncodingWithMarkers;
        static GCEncoding _utf8EncodingWithoutMarkers;
#pragma warning(default:4251)
      public:
        virtual ~Encoding();
  
        // Methods
        // GetByteCount
        virtual int GetByteCount(CharArray&, int, int) = 0;
        virtual int GetByteCount(String&);
        virtual int GetByteCount(CharArray&);
        virtual int GetByteCount(wchar_t*, int);
        
        // GetBytes
        virtual int GetBytes(CharArray&, int, int, ByteArray&, int) = 0;
        virtual int GetBytes(String&, int, int, ByteArray&, int);
        virtual int GetBytes(wchar_t*, int, byte*, int);
        virtual ByteArray GetBytes(String&);
        virtual ByteArray GetBytes(CharArray&, int, int);
        virtual ByteArray GetBytes(CharArray&);

        // GetCharCount
        virtual int GetCharCount(ByteArray&s, int, int) = 0;
        virtual int GetCharCount(ByteArray&);
        virtual int GetCharCount(byte*, int);

        virtual int GetChars(ByteArray&, int, int, CharArray&, int) = 0;
        virtual CharArray GetChars(ByteArray&);
        virtual CharArray GetChars(ByteArray&, int, int);
        virtual int GetChars(byte*, int, wchar_t*, int);
       
        virtual int GetMaxByteCount(int) = 0;
        virtual int GetMaxCharCount(int byteCount) = 0;
        virtual System::ByteArray GetPreamble();

        virtual String GetString(ByteArray& bytes, int index, int count);
        virtual String GetString(ByteArray& bytes);
        
        // Properties
        virtual bool IsSingleByte();
        virtual int WindowsCodePage();

        virtual Decoder* GetDecoder();

        static GCEncoding& Default();
        static GCEncoding& BigEndianUnicode();
        static GCEncoding& Unicode();
        static GCEncoding& UTF8();
        static GCEncoding GetEncoding(int);
        static GCEncoding GetEncoding(String& name);

      protected:
        Encoding(int codePage = 0);
        static GCEncoding& UTF8Unmarked();
        static String InternalCodePage(int& code_page);
        static Threading::CriticalSection _lockobj;

      friend IO::StreamReader;
      };
    }
  }
#pragma warning(default:4266)
