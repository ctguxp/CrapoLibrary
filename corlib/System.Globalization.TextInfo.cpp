#include "pch.h"
#include "System.Globalization.TextInfo.h"
#include "System.String.h"

namespace System
  {
  namespace Globalization
    {
    TextInfo::TextInfo(CultureInfo* ci, int lcid, void* data, bool read_only)
      :_isReadOnly(read_only)
      ,_win32LangID(lcid)
      ,_ci(ci)
      {

      if(data != nullptr)
        _data = *(Data*)data;
      else 
        {
        //this.data = new Data ();
        _data.list_sep = (byte)L',';
        }

      //CultureInfo tmp = ci;
      //while (tmp.Parent != null && tmp.Parent.LCID != 0x7F && tmp.Parent != tmp)
      //  tmp = tmp.Parent;

      //if (tmp != null) 
      //  {
      //  switch (tmp.LCID) {
      //    case 44: // Azeri (az)
      //    case 31: // Turkish (tr)
      //      handleDotI = true;
      //      break;
      //    }
      //  }
      }
    TextInfo::~TextInfo()
      {
      }

    String TextInfo::ToLower(String str)
      {
      // In ICU (3.2) there are a few cases that one single
      // character results in multiple characters in e.g.
      // tr-TR culture. So I tried brute force conversion
      // test with single character as a string input, but 
      // there was no such conversion. So I think it just
      // invokes ToLower(char).

      if(str.Length() == 0)
        return String::Empty();

      String tmp = String::InternalAllocateStr(str.Length());
      const wchar_t* source = (cstring)str;
      const wchar_t* dest = (cstring)tmp;

      wchar_t* destPtr = (string)dest;
      wchar_t* sourcePtr = (string)source;

      for(int n = 0; n < str.Length(); n++)
        {
        *destPtr = ToLower(*sourcePtr);
        sourcePtr++;
        destPtr++;
        }
      return tmp;
      }

    wchar_t TextInfo::ToLower(wchar_t c)
      {
      // quick ASCII range check
      if(c < 0x40 || 0x60 < c && c < 128)
        return c;
      //else if (L'A' <= c && c <= L'Z' && (!handleDotI || c != L'I'))
      return (wchar_t)(c + 0x20);

      //if(ci == nullptr || ci->LCID() == 0x7F)
      //  return Char::ToLowerInvariant(c);

      //switch (c) 
      //  {
      //  case '\u0049': // Latin uppercase I
      //    if (handleDotI)
      //      return '\u0131'; // I becomes dotless i
      //    break;
      //  case '\u0130': // I-dotted
      //    return '\u0069'; // i

      //  case '\u01c5': // LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON
      //    return '\u01c6';
      //    // \u01c7 -> \u01c9 (LJ) : invariant
      //  case '\u01c8': // LATIN CAPITAL LETTER L WITH SMALL LETTER J
      //    return '\u01c9';
      //    // \u01ca -> \u01cc (NJ) : invariant
      //  case '\u01cb': // LATIN CAPITAL LETTER N WITH SMALL LETTER J
      //    return '\u01cc';
      //    // WITH CARON : invariant
      //    // WITH DIAERESIS AND * : invariant

      //  case '\u01f2': // LATIN CAPITAL LETTER D WITH SMALL LETTER Z
      //    return '\u01f3';
      //  case '\u03d2':  // ? it is not in ICU
      //    return '\u03c5';
      //  case '\u03d3':  // ? it is not in ICU
      //    return '\u03cd';
      //  case '\u03d4':  // ? it is not in ICU
      //    return '\u03cb';
      //  }
      //return Char.ToLowerInvariant (c);
      }
    }
  }
