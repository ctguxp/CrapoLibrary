// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"
#include "System.IFormatProvider.h"
#include "System.Globalization.h"

namespace System
  {
  class Exception;
  class CRAPOCOREDLL_API Int32 : public Object, public IComparable
    {
    public:
      static const int32 MaxValue = 2147483647;
      static const int32 MinValue = (-2147483647 - 1);
    private:
      int32 _value;
    public:
      Int32();
      Int32(int32 const&);
      ~Int32();
      Int32& operator = (int32 const&);
      operator int32 const& () const;
      virtual int CompareTo(Object& /*value*/) override;
      virtual String ToString() override;
      String ToString(String /*format*/);
      String ToString(IFormatProvider* /*provider*/);
      String ToString(String format, IFormatProvider* /*provider*/);
      virtual uint32 GetHashCode() override;
      static Exception GetFormatException();
      virtual bool Equals(Object* /*obj*/) override;
      static int Parse(String /*s*/);
      static bool CheckStyle(Globalization::NumberStyles style, bool tryParse, Exception& exc);
      static void FindCurrency(int32& pos, String s, Globalization::NumberFormatInfo* nfi, bool& foundCurrency);
      static void FindSign(int32& pos, String s, Globalization::NumberFormatInfo* nfi, bool& foundSign, bool& negative);
      static bool FindOther(int32 pos, String s, String other);
      static bool ValidDigit(wchar_t e, bool allowHex);
      static bool JumpOverWhite(int32& pos, String s, bool reportError, bool tryParse, Exception& exc); 
    private:
      static bool Parse(String /*s*/, bool /*tryParse*/, int& /*result*/, Exception& /*exc*/);
      static bool ProcessTrailingWhitespace(bool /*tryParse*/, String /*s*/, int /*position*/, Exception& /*exc*/);
    };
  }

