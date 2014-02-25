// Perch Lake Computer System

#include "pch.h"
#include "System.Char.h"
#include "System.Exception.h"
#include "System.Windows.Locale.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Globalization.UnicodeCategory.h"
#include "CharConversions.h"

namespace System
  {
  // Default constructor
  Char::Char()
    :_datum()
    {
    }
  // Constructor takes datum
  Char::Char(wchar_t const& d)
    :_datum(d)
    {
    }
  // Destructor
  Char::~Char()
    {
    }
  // Assignment operator
  Char& Char::operator = (wchar_t const& d)
    {
    _datum = d;
    return *this;
    }
  // Member operator
  Char::operator wchar_t const& () const
    {
    return _datum;
    }

  bool Char::Equals(Char other)
    {
    return _datum == other._datum;
    }
  // Is low surrogate
  bool Char::IsLowSurrogate(wchar_t c)
    {
    return c >= 0xDC00 && c <= 0xDFFF;
    }
  // Is high surrogate
  bool Char::IsHighSurrogate(wchar_t c) 
    {
    return c >= 0xD800 && c <= 0xDBFF;
    }
  // Is surrogate
  bool Char::IsSurrogate(wchar_t c)
    {
    return c >= 0xD800 && c <= 0xDFFF;
    }

  bool Char::IsWhiteSpace(wchar_t c)
    {
    using namespace Globalization;
    if(c < 0x1680)
      return c == 0x20 || c >= 0x09 && c <= 0x0d || c == 0x85 || c == 0xA0;

    int category = CategoryData_v4[c];
    return category > (int)UnicodeCategory::OtherNumber && category <= (int) UnicodeCategory::ParagraphSeparator;
    }

  wchar_t Char::ToLower(wchar_t c)
    {
    // CurrentCulture is never null or Invariant
    return Globalization::CultureInfo::CurrentCulture().TextInfo().ToLower(c);
    }

  bool Char::IsLetter(wchar_t c)
    {
    using namespace Globalization;
    return CategoryData_v4[c] <= ((byte)UnicodeCategory::OtherLetter);
    }

  wchar_t Char::ToUpperInvariant(wchar_t c)
    {
    if (c <= ((wchar_t)0x24e9))
      return (wchar_t)ToUpperDataLow[c];
    if (c >= ((wchar_t)0xff21))
      return (wchar_t)ToUpperDataHigh[c - 0xff21];
    return c;
    }

  String Char::ToString()
    {
    return String(_datum, 1);
    }

  uint32 Char::GetHashCode()
    {
    return abs(_datum);
    }

  void Char::CheckParameter(String& s, int index)
    {
    /*if(s.IsNull()) 
    throw ArgumentNullException(L"s");*/

    if(index < 0 || index >= (int)s.Length()) 
      throw ArgumentOutOfRangeException(Locale::GetText(L"The value of index is less than zero, or greater than or equal to the length of s."));
    }
  }
