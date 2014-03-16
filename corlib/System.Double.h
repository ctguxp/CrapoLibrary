// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.IFormatProvider.h"
#include "System.Exception.h"
#include "System.Globalization.NumberStyles.h"

namespace System
  {
  class CRAPOCOREDLL_API Double : public Object
    {
    enum class ParseState
    {
			None = 0,
      AllowSign = 1,
			Digits = 2,
			Decimal = 3,
			ExponentSign = 4,
			Exponent = 5,
			ConsumeWhiteSpace = 6,
			TrailingSymbols = 7,
			Exit = 8
		};
    public:
        static const double NaN;
        static const double MaxValue;
        static const double MinValue;
        static const double NegativeInfinity;
		    static const double PositiveInfinity;
    private:
      double _value;
    private:
      struct ParseStruct
      {
      bool tryParse;
      bool allow_trailing_parenthes;
      bool allow_trailing_white;
      int didx;
      int sidx;
      int currency_symbol_len;
      ParseState state;               
      Globalization::NumberStyles style;
      wchar_t c;
      byte*   b;
      };
    public:
      Double();
      Double(double const&);
      ~Double();
      Double& operator = (double const&);
      operator double const& () const;
      virtual uint32 GetHashCode() override;
      virtual String ToString() override;
      String ToString(IFormatProvider*);
      String ToString(String);
      String ToString(String, IFormatProvider*);
      static bool IsNegativeInfinity(double);
      static bool IsPositiveInfinity(double);
      static double Parse(String, IFormatProvider*);
      static double Parse(String, Globalization::NumberStyles, IFormatProvider*);
    private:
      static bool Parse(String s, Globalization::NumberStyles style, IFormatProvider* provider, bool tryParse, double& result, Exception& exc);
      static bool TryParseStringConstant(String format, String s, int start, int end);
      static bool ParseImpl(byte* byte_ptr, double& value);
      static int Double::ParseDecimal(ParseStruct&, Exception& exc);
      static int Double::ParseExponent(ParseStruct&);
      static int Double::ParseTrailingSymbols(ParseStruct&, String& s, String& positive, String& negative, String& currencySymbol);
      static int ConsumeWhiteSpace(ParseStruct&, Exception& exc);
    };
  }

