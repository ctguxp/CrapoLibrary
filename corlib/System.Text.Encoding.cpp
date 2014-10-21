// Perch Lake Computer System

#include "pch.h"
#include "System.Text.Encoding.h"
#include "System.Text.DecoderFallback.h"
#include "System.Text.EncoderFallback.h"
#include "System.Exception.h"
#include "Encodings.h"
#include "System.Text.ASCIIEncoding.h"
#include "System.Text.Unicode.h"
#include "System.Text.UTF8Encoding.h"
#include "System.Threading.Lock.h"

#include "I18N.West.CP1252.h"

namespace System
  {
  namespace Text
    {

    GCEncoding Encoding::_defaultEncoding;
    GCEncoding Encoding::_unicodeEncoding;
    GCEncoding Encoding::_bigEndianEncoding;
    GCEncoding Encoding::_utf8EncodingWithMarkers;
    GCEncoding Encoding::_utf8EncodingWithoutMarkers;
    Threading::CriticalSection Encoding::_lockobj;

    // Default constructor
    Encoding::Encoding(int codePage)
      :_codePage(codePage)
      ,_windows_code_page(_codePage)
      ,_is_readonly(true)
      ,_decoder_fallback(nullptr)
      ,_encoder_fallback(nullptr)
      {
      switch(_codePage)
        {
        case 1200:  // UTF16
        case 1201:  // UTF16
        case 65001: // UTF8
          _decoder_fallback = &DecoderFallback::StandardSafeFallback();
          _encoder_fallback = &EncoderFallback::StandardSafeFallback();
          break;
        case 20127: // ASCII
        case 54936: // GB18030
        default:
          _decoder_fallback = &DecoderFallback::ReplacementFallback();
          _encoder_fallback = &EncoderFallback::ReplacementFallback();
        }
      }
    // Destructor
    Encoding::~Encoding()
      {
      }

    bool Encoding::IsSingleByte()
      {
      return false;
      }

    int Encoding::GetByteCount(String& s)
      {
      /*if(s.IsNull())
      throw ArgumentNullException(L"s");*/

      if(s.Length() == 0)
        return 0;
      cstring cptr = s;
        {
        return GetByteCount((wstring)cptr, s.Length());
        }
      }

    int Encoding::GetByteCount(CharArray& chars)
      {
      if(!chars.IsNull())
        {
        return GetByteCount(chars, 0, (int)chars.Length());
        }
      else
        {
        throw ArgumentNullException(L"chars");
        }
      }

    int Encoding::GetByteCount(wchar_t* chars, int count)
      {
      if(chars == nullptr)
        throw ArgumentNullException(L"chars");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count");
      CharArray c(count);

      for(int p = 0; p < count; p++)
        c[p] = chars[p];

      return GetByteCount(c);
      }

    ByteArray Encoding::GetBytes(String& s)
      {
      /*if(s.IsNull())
      throw ArgumentNullException(L"s");*/

      if (s.Length() == 0)
        return ByteArray();
      int byteCount = GetByteCount(s);
      if(byteCount == 0)
        return ByteArray();
      cstring cptr = s;
      ByteArray bytes(byteCount, L'\0');
      byte* bptr = bytes.ToPtr();
      GetBytes((wstring)cptr, s.Length(), bptr, byteCount);
      return bytes;
      }

    ByteArray Encoding::GetBytes(CharArray& chars)
      {
      int numBytes = GetByteCount(chars, 0, (int)chars.Length());
      ByteArray bytes(numBytes);
      GetBytes(chars, 0, (int)chars.Length(), bytes, 0);
      return bytes;
      }

    int Encoding::GetBytes(String& s, int charIndex, int charCount, ByteArray& bytes, int byteIndex)
      {
      /*if(s.IsNull())
      throw ArgumentNullException(L"s");*/
      if(charIndex < 0 || charIndex > (int)s.Length())
        throw ArgumentOutOfRangeException(L"charIndex", L"Argument index is out of array range.");
      if (charCount < 0 || charIndex > ((int)s.Length() - charCount))
        throw ArgumentOutOfRangeException(L"charCount", L"Argument index is out of array range.");
      if (byteIndex < 0 || byteIndex > (int)bytes.Length())
        throw ArgumentOutOfRangeException(L"byteIndex", L"Argument index is out of array range.");

      if (charCount == 0 || (int)bytes.Length() == byteIndex)
        return 0;
      cstring cptr = s;
      byte* bptr = bytes.ToPtr();
      return GetBytes((wstring)cptr + charIndex, charCount, bptr + byteIndex, (int)bytes.Length() - byteIndex);
      }

    int Encoding::GetBytes(wchar_t *chars, int charCount, byte* bytes, int byteCount)
      {
      if (bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      if (chars == nullptr)
        throw ArgumentNullException(L"chars");
      if(charCount < 0)
        throw ArgumentOutOfRangeException(L"charCount");
      if(byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount");

      CharArray c(charCount);

      for(int i = 0; i < charCount; i++)
        c [i] = chars [i];

      ByteArray b = GetBytes(c, 0, charCount);
      int top = (int)b.Length();
      if(top > byteCount)
        throw ArgumentException(L"byteCount is less that the number of bytes produced", L"byteCount");

      for(int i = 0; i < top; i++)
        bytes [i] = b [i];

      return (int)b.Length();
      }

    ByteArray Encoding::GetBytes(CharArray& chars, int index, int count)
      {
      int numBytes = GetByteCount(chars, index, count);
      ByteArray bytes(numBytes);
      GetBytes(chars, index, count, bytes, 0);
      return bytes;
      }


    // When overridden in a derived class, returns a sequence of bytes that specifies the encoding used.
    System::ByteArray Encoding::GetPreamble()
      {
      return System::ByteArray();
      }

    String Encoding::GetString(ByteArray& bytes, int index, int count)
      {
      return String(GetChars(bytes, index, count));
      }

    String Encoding::GetString(ByteArray& bytes)
      {
      if(bytes.IsNull())
        throw ArgumentNullException(L"bytes");

      return GetString(bytes, 0, (int)bytes.Length());
      }

    int Encoding::GetCharCount(ByteArray& bytes)
      {
      if(bytes.IsNull())
        {
        throw ArgumentNullException(L"bytes");
        }
      return GetCharCount (bytes, 0, (int)bytes.Length());
      }

    int Encoding::GetCharCount(byte *bytes, int count)
      {
      using namespace System;
      if(bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count");

      ByteArray ba(count);
      for(int i = 0; i < count; i++)
        ba [i] = bytes[i];
      return GetCharCount(ba, 0, count);
      }

    CharArray Encoding::GetChars(ByteArray& bytes)
      {
      if(bytes.IsNull())
        {
        throw ArgumentNullException(L"bytes");
        }
      int numChars = GetCharCount(bytes, 0, (int)bytes.Length());
      CharArray chars(numChars);
      GetChars(bytes, 0, (int)bytes.Length(), chars, 0);
      return chars;
      }

    CharArray Encoding::GetChars(ByteArray& bytes, int index, int count)
      {
      int numChars = GetCharCount(bytes, index, count);
      CharArray chars(numChars);
      GetChars(bytes, index, count, chars, 0);
      return chars;
      }

    int Encoding::GetChars(byte* bytes, int byteCount, wchar_t* chars, int charCount)
      {
      if (bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      if (chars == nullptr)
        throw ArgumentNullException(L"chars");
      if (charCount < 0)
        throw ArgumentOutOfRangeException(L"charCount");
      if (byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount");

      ByteArray ba(byteCount);
      for(int i = 0; i < byteCount; i++)
        ba[i] = bytes[i];
      CharArray ret = GetChars(ba, 0, byteCount);
      int top = (int)ret.Length();

      if (top > charCount)
        throw ArgumentException(L"charCount is less than the number of characters produced", L"charCount");

      for (int i = 0; i < top; i++)
        chars[i] = ret[i];
      return top;
      }

    String Encoding::InternalCodePage(int& code_page)
      {
      GCString temp(GetCodePage(code_page));
      if(temp.Get() != nullptr)
        return *temp.Get();
      return String();
      }

    GCEncoding& Encoding::Default()
      {
      if(_defaultEncoding.Get() == nullptr) 
        {
        Threading::Lock lock(_lockobj);
          {
          if(_defaultEncoding.Get() == nullptr)
            {
            // See if the underlying system knows what
            // code page handler we should be using.
            int code_page = 1;

            String code_page_name = InternalCodePage(code_page);
            try 
              {
              if (code_page == -1)
                _defaultEncoding = GetEncoding(code_page_name);
              else {
                // map the codepage from internal to our numbers
                code_page = code_page & 0x0fffffff;
                switch (code_page)
                  {
                  case 1: code_page = ASCIIEncoding::ASCII_CODE_PAGE; break;
                    //case 2: code_page = UTF7Encoding.UTF7_CODE_PAGE; break;
                  case 3: code_page = UTF8Encoding::UTF8_CODE_PAGE; break;
                  case 4: code_page = UnicodeEncoding::UNICODE_CODE_PAGE; break;
                  case 5: code_page = UnicodeEncoding::BIG_UNICODE_CODE_PAGE; break;
                    //case 6: code_page = Latin1Encoding.ISOLATIN_CODE_PAGE; break;
                  }
                _defaultEncoding = GetEncoding(code_page);
                }
              } 
            //catch (NotSupportedException) 
            //{
            // code_page is not supported on underlying platform
            //defaultEncoding = UTF8Unmarked;
            //} 
            catch(ArgumentException&) 
              {
              // code_page_name is not a valid code page, or is 
              // not supported by underlying OS
              //defaultEncoding = UTF8Unmarked;
              }
            //defaultEncoding.is_readonly = true;
            }
          }
        }
      return _defaultEncoding;
      }

#pragma warning(disable:4100)
    GCEncoding Encoding::GetEncoding(String& name)
      {
      return GCEncoding((Encoding*)new I18N::West::CP1252());
      //String converted = name.ToLowerInvariant ().Replace ('-', '_');

      // Builtin web encoding names and the corresponding code pages.
      //switch(converted)
      //  {
      //  case "ascii":
      //  case "us_ascii":
      //  case "us":
      //  case "ansi_x3.4_1968":
      //  case "ansi_x3.4_1986":
      //  case "cp367":
      //  case "csascii":
      //  case "ibm367":
      //  case "iso_ir_6":
      //  case "iso646_us":
      //  case "iso_646.irv:1991":
      //    //return GetEncoding (ASCIIEncoding.ASCII_CODE_PAGE);

      //  case "utf_7":
      //  case "csunicode11utf7":
      //  case "unicode_1_1_utf_7":
      //  case "unicode_2_0_utf_7":
      //  case "x_unicode_1_1_utf_7":
      //  case "x_unicode_2_0_utf_7":
      //    //return GetEncoding (UTF7Encoding.UTF7_CODE_PAGE);

      //  case "utf_8":
      //  case "unicode_1_1_utf_8":
      //  case "unicode_2_0_utf_8":
      //  case "x_unicode_1_1_utf_8":
      //  case "x_unicode_2_0_utf_8":
      //    //return GetEncoding (UTF8Encoding.UTF8_CODE_PAGE);

      //  case "utf_16":
      //  case "utf_16le":
      //  case "ucs_2":
      //  case "unicode":
      //  case "iso_10646_ucs2":
      //    //return GetEncoding (UnicodeEncoding.UNICODE_CODE_PAGE);

      //  case "unicodefffe":
      //  case "utf_16be":
      //    //return GetEncoding (UnicodeEncoding.BIG_UNICODE_CODE_PAGE);

      //  case "utf_32":
      //  case "utf_32le":
      //  case "ucs_4":
      //    //return GetEncoding (UTF32Encoding.UTF32_CODE_PAGE);

      //  case "utf_32be":
      //    //return GetEncoding (UTF32Encoding.BIG_UTF32_CODE_PAGE);

      //  case "iso_8859_1":
      //  case "latin1":
      //    //return GetEncoding (Latin1Encoding.ISOLATIN_CODE_PAGE);
      //  }

      // Try to obtain a web encoding handler from the I18N handler.
      /*Encoding enc = (Encoding)(InvokeI18N ("GetEncoding", name));
      if (enc != null) 
      {
      return enc;
      }*/

      // Build a web encoding class name.
      //String encName = "System.Text.ENC" + converted;


      // Look for a code page converter in this assembly.
      /*Assembly assembly = Assembly.GetExecutingAssembly ();
      Type type = assembly.GetType (encName);
      if (type != null) 
      {
      return (Encoding)(Activator.CreateInstance (type));
      }*/

      // Look in any assembly, in case the application
      // has provided its own code page handler.
      /*type = Type.GetType (encName);
      if (type != null) 
      {
      return (Encoding)(Activator.CreateInstance (type));
      }*/
      // We have no idea how to handle this encoding name.
      //throw new ArgumentException (String.Format("Encoding name '{0}' not " + "supported", name), "name");
      }
#pragma warning(disable:4100)

    GCEncoding Encoding::GetEncoding(int codepage)
      {
      if(codepage < 0 || codepage > 0xffff)
        throw ArgumentOutOfRangeException(L"codepage",  L"Valid values are between 0 and 65535, inclusive.");

      //Check for the builtin code pages first.
      switch(codepage)
        {
        case 0: 
          return Default();
        case ASCIIEncoding::ASCII_CODE_PAGE:
          
          return GCEncoding((Encoding*)new ASCIIEncoding);
          //case UTF7Encoding.UTF7_CODE_PAGE:
          //return UTF7;
        case UTF8Encoding::UTF8_CODE_PAGE:
          return GCEncoding((Encoding*)new UTF8Encoding);
          //case UTF32Encoding.UTF32_CODE_PAGE:
          //return UTF32;

          //case UTF32Encoding.BIG_UTF32_CODE_PAGE:
          //return BigEndianUTF32;

        case UnicodeEncoding::UNICODE_CODE_PAGE:
          return Unicode();

        case UnicodeEncoding::BIG_UNICODE_CODE_PAGE:
          return BigEndianUnicode();

          //case Latin1Encoding.ISOLATIN_CODE_PAGE:
          //return ISOLatin1;
        default: break;
        }
      return GCEncoding();
      // Try to obtain a code page handler from the I18N handler.
      /*Encoding enc = (Encoding)(InvokeI18N ("GetEncoding", codepage));
      if (enc != null) 
      {
      enc.is_readonly = true;
      return enc;
      }*/

      // Build a code page class name.
      //String cpName = "System.Text.CP" + codepage.ToString();

      // Look for a code page converter in this assembly.
      /*Assembly assembly = Assembly.GetExecutingAssembly ();
      Type type = assembly.GetType (cpName);
      if (type != null) 
      {
      enc = (Encoding)(Activator.CreateInstance (type));
      enc.is_readonly = true;
      return enc;
      }*/

      // Look in any assembly, in case the application
      // has provided its own code page handler.
      /*type = Type.GetType (cpName);
      if (type != null) 
      {
      enc = (Encoding)(Activator.CreateInstance (type));
      enc.is_readonly = true;
      return enc;
      }*/
      // We have no idea how to handle this code page.
      //throw new NotSupportedException(String.Format ("CodePage {0} not supported", codepage.ToString ()));
      }

    int Encoding::WindowsCodePage()
      {
      // We make no distinction between normal and
      // Windows code pages in this implementation.
      return _windows_code_page;
      }

    GCEncoding& Encoding::BigEndianUnicode()
      {
      if(_bigEndianEncoding.Get() == nullptr) 
        {
        Threading::Lock lock(_lockobj);
          {
          if(Encoding::_bigEndianEncoding.Get() == nullptr) 
            {
            Encoding::_bigEndianEncoding.Reset((Encoding*)new UnicodeEncoding (true, true));
            }
          }
        }

      return _bigEndianEncoding;
      }

    GCEncoding& Encoding::Unicode()
      {
      if(_unicodeEncoding.Get() == nullptr) 
        {
        Threading::Lock lock(_lockobj);
          {
          if(Encoding::_unicodeEncoding.Get() == nullptr)
            {
            Encoding::_unicodeEncoding.Reset((Encoding*)new UnicodeEncoding(false,true));
            }
          }
        }

      return Encoding::_unicodeEncoding;
      }

    GCEncoding& Encoding::UTF8()
      {
      if(_utf8EncodingWithMarkers.Get() == nullptr)
        {
        Threading::Lock lock(_lockobj);
          {
          if(Encoding::_utf8EncodingWithMarkers.Get() == nullptr)
            {
            Encoding::_utf8EncodingWithMarkers.Reset((Encoding*)new UTF8Encoding(true));
            }
          }
        }
      return Encoding::_utf8EncodingWithMarkers;
      }

    GCEncoding& Encoding::UTF8Unmarked() 
      {
      if(_utf8EncodingWithoutMarkers.Get() == nullptr) 
        {
        Threading::Lock lock(_lockobj);
          {
          if(Encoding::_utf8EncodingWithoutMarkers.Get() == nullptr)
            {
            _utf8EncodingWithoutMarkers.Reset(new UTF8Encoding(false, false));
            }
          }
        }

      return Encoding::_utf8EncodingWithoutMarkers;
      }

    Decoder* Encoding::GetDecoder()
      {
      return (Decoder*)new ForwardingDecoder(this);
      }

    Encoding::ForwardingDecoder::ForwardingDecoder(Encoding* enc)
      :_encoding(enc)
      {
      }

    int Encoding::ForwardingDecoder::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex)
      {
      return _encoding->GetChars(bytes, byteIndex, byteCount, chars, charIndex);
      }
    }
  }
