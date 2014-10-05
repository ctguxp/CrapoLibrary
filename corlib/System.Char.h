// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"
#include "System.IEquatable.h"
#include "System.Globalization.UnicodeCategory.h"

namespace System
  {
  class CRAPOCOREDLL_API Char : public Object, public IEquatable<Char>
    {
    private:
      wchar_t _datum;
    public:
      static const wchar_t MinValue = (wchar_t)0x0000;
      static const wchar_t MaxValue = (wchar_t)0xFFFF;
      Char();
      Char(wchar_t const&);
      ~Char();
      Char& operator = (wchar_t const&);
      operator wchar_t const& () const;
      virtual bool Equals(Object* obj) override;
      virtual bool Equals(Char) override;
      static bool IsLowSurrogate(wchar_t);
      static bool IsHighSurrogate(wchar_t);
      static bool IsSurrogate(wchar_t);
      static bool IsWhiteSpace(wchar_t);
      static bool IsDigit(wchar_t c);
      static bool IsLetter(wchar_t c);
      static bool IsLetterOrDigit(wchar_t c);
      static Globalization::UnicodeCategory GetUnicodeCategory(wchar_t c);
      static wchar_t ToLower(wchar_t c);
      static wchar_t ToLowerInvariant(wchar_t c);
      static wchar_t ToUpperInvariant(wchar_t c);
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    private:
      static void CheckParameter(String& s, int index);
    };
  }
