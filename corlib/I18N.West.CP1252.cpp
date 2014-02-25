// Perch Lake Computer System

#include "pch.h"
#include "I18N.West.CP1252.h"
#include "System.Exception.h"
#include "System.Text.EncoderFallbackBuffer.h"

namespace I18N
  {
  namespace West
    {
    wchar_t CP1252::ToChars[256] = { 
      0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
      0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 
      0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011, 
      0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 
      0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 
      0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023, 
      0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 
      0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 
      0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 
      0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 
      0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 
      0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 
      0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 
      0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 
      0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 
      0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, 
      0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 
      0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 
      0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 
      0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 
      0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 
      0x007E, 0x007F, 0x20AC, 0x0081, 0x201A, 0x0192, 
      0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 
      0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F, 
      0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 
      0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 
      0x0153, 0x009D, 0x017E, 0x0178, 0x00A0, 0x00A1, 
      0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 
      0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 
      0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3, 
      0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 
      0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF, 
      0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 
      0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 
      0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 
      0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 
      0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 
      0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 
      0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 
      0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 
      0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 
      0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 
      0x00FC, 0x00FD, 0x00FE, 0x00FF };

    CP1252::CP1252()
      :Common::ByteEncoding(1252, System::CharArray((wchar_t*)CP1252::ToChars, 256), L"Western European (Windows)", L"iso-8859-1", L"Windows-1252", L"Windows-1252", true, true, true, true, 1252)
      {
      }
    CP1252::~CP1252()
      {
      }

    int CP1252::GetByteCountImpl(wchar_t* chars, int count)
      {
      if(_encoder_fallback != nullptr)		
        {
        //Calculate byte count by actually doing encoding and discarding the data.
        return GetBytesImpl(chars, count, nullptr, 0);
        }
      else
        {
        return count;
        }
      }

    // ------------------------------------------------------------------------
    // GetByteCount function
    int CP1252::GetByteCount(String& s)
      {
      if(_encoder_fallback != nullptr)
        {
        //Calculate byte count by actually doing encoding and discarding the data.
        cstring s_ptr = s;
        return GetBytesImpl((string)s_ptr, s.Length(), nullptr, 0);
        }
      else
        {
        //byte count equals character count because no EncoderFallback set
        return s.Length();
        }
      }
    int CP1252::GetByteCount(CharArray& chars)
      {
      return System::Text::Encoding::GetByteCount(chars);
      }   
    int CP1252::GetByteCount(wchar_t* chars, int count)
      {
      return System::Text::Encoding::GetByteCount(chars, count);
      }
    // ------------------------------------------------------------------------

    void CP1252::ToBytes(wchar_t* chars, int charCount, byte* bytes, int byteCount)
      {
      //Calling ToBytes with null destination buffer doesn't make any sense
      if(bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      GetBytesImpl(chars, charCount, bytes, byteCount);
      }

    int CP1252::GetBytesImpl(wchar_t* chars, int charCount, byte* bytes, int byteCount)
      {
      using namespace System;
      int ch;
      int charIndex = 0;
      int byteIndex = 0;
      Text::GCEncoderFallbackBuffer buffer;
      while (charCount > 0)
        {
        ch = (int)(chars[charIndex]);
        charIndex++;
        charCount--;
        if(ch >= 128) switch(ch)
          {
          case 0x0081:
          case 0x008D:
          case 0x008F:
          case 0x0090:
          case 0x009D:
          case 0x00A0:
          case 0x00A1:
          case 0x00A2:
          case 0x00A3:
          case 0x00A4:
          case 0x00A5:
          case 0x00A6:
          case 0x00A7:
          case 0x00A8:
          case 0x00A9:
          case 0x00AA:
          case 0x00AB:
          case 0x00AC:
          case 0x00AD:
          case 0x00AE:
          case 0x00AF:
          case 0x00B0:
          case 0x00B1:
          case 0x00B2:
          case 0x00B3:
          case 0x00B4:
          case 0x00B5:
          case 0x00B6:
          case 0x00B7:
          case 0x00B8:
          case 0x00B9:
          case 0x00BA:
          case 0x00BB:
          case 0x00BC:
          case 0x00BD:
          case 0x00BE:
          case 0x00BF:
          case 0x00C0:
          case 0x00C1:
          case 0x00C2:
          case 0x00C3:
          case 0x00C4:
          case 0x00C5:
          case 0x00C6:
          case 0x00C7:
          case 0x00C8:
          case 0x00C9:
          case 0x00CA:
          case 0x00CB:
          case 0x00CC:
          case 0x00CD:
          case 0x00CE:
          case 0x00CF:
          case 0x00D0:
          case 0x00D1:
          case 0x00D2:
          case 0x00D3:
          case 0x00D4:
          case 0x00D5:
          case 0x00D6:
          case 0x00D7:
          case 0x00D8:
          case 0x00D9:
          case 0x00DA:
          case 0x00DB:
          case 0x00DC:
          case 0x00DD:
          case 0x00DE:
          case 0x00DF:
          case 0x00E0:
          case 0x00E1:
          case 0x00E2:
          case 0x00E3:
          case 0x00E4:
          case 0x00E5:
          case 0x00E6:
          case 0x00E7:
          case 0x00E8:
          case 0x00E9:
          case 0x00EA:
          case 0x00EB:
          case 0x00EC:
          case 0x00ED:
          case 0x00EE:
          case 0x00EF:
          case 0x00F0:
          case 0x00F1:
          case 0x00F2:
          case 0x00F3:
          case 0x00F4:
          case 0x00F5:
          case 0x00F6:
          case 0x00F7:
          case 0x00F8:
          case 0x00F9:
          case 0x00FA:
          case 0x00FB:
          case 0x00FC:
          case 0x00FD:
          case 0x00FE:
          case 0x00FF:
            break;
          case 0x0152: ch = 0x8C; break;
          case 0x0153: ch = 0x9C; break;
          case 0x0160: ch = 0x8A; break;
          case 0x0161: ch = 0x9A; break;
          case 0x0178: ch = 0x9F; break;
          case 0x017D: ch = 0x8E; break;
          case 0x017E: ch = 0x9E; break;
          case 0x0192: ch = 0x83; break;
          case 0x02C6: ch = 0x88; break;
          case 0x02DC: ch = 0x98; break;
          case 0x2013: ch = 0x96; break;
          case 0x2014: ch = 0x97; break;
          case 0x2018: ch = 0x91; break;
          case 0x2019: ch = 0x92; break;
          case 0x201A: ch = 0x82; break;
          case 0x201C: ch = 0x93; break;
          case 0x201D: ch = 0x94; break;
          case 0x201E: ch = 0x84; break;
          case 0x2020: ch = 0x86; break;
          case 0x2021: ch = 0x87; break;
          case 0x2022: ch = 0x95; break;
          case 0x2026: ch = 0x85; break;
          case 0x2030: ch = 0x89; break;
          case 0x2039: ch = 0x8B; break;
          case 0x203A: ch = 0x9B; break;
          case 0x20AC: ch = 0x80; break;
          case 0x2122: ch = 0x99; break;
          default:
            {
            if(ch >= 0xFF01 && ch <= 0xFF5E)
              {
              ch -= 0xFEE0;
              }
            else
              {
              HandleFallback(buffer, chars, charIndex, charCount, bytes, byteIndex, byteCount);
              continue;
              }
            }
            break;
          }
        //Write encoded byte to buffer, if buffer is defined and fallback was not used
        if (bytes != nullptr)
          bytes[byteIndex] = (byte)ch;
        byteIndex++;
        byteCount--;
        }
      return byteIndex;
      }
    }
  }
