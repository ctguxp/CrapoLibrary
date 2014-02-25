// Perch Lake Computer System

#pragma once
#include "System.IFormatProvider.h"
#include "System.String.h"
#include "AutoPtr.h"
#include "System.Array.hpp"

namespace System
  {
  class NumberFormatter;
  namespace Globalization
    {
    class CultureInfo;
    class NumberFormatInfo : public IFormatProvider
      {
      public:
        NumberFormatInfo();
        virtual ~NumberFormatInfo();
        virtual Object GetFormat() override;
        static NumberFormatInfo* GetInstance(IFormatProvider*);
        static NumberFormatInfo* CurrentInfo();
        String NegativeSign();
        String CurrencySymbol();
        void CurrencySymbol(cstring value);
        String CurrencyDecimalSeparator();
        int CurrencyDecimalDigits() { return _currencyDecimalDigits; }
        String CurrencyGroupSeparator();
        int CurrencyNegativePattern() { return _currencyNegativePattern; }
        int CurrencyPositivePattern() { return _currencyPositivePattern; }
        String NaNSymbol();
        String NegativeInfinitySymbol();
        String PositiveInfinitySymbol();
        String NumberDecimalSeparator();
      private:
        NumberFormatInfo(int, bool);
        IntArray& RawCurrencyGroupSizes() { return _currencyGroupSizes; }
      private:
        bool     _isReadOnly;

        // Currency Related Format Info
        int      _currencyDecimalDigits;
        String   _currencyDecimalSeparator;
        String   _currencyGroupSeparator;
        IntArray _currencyGroupSizes;
        int      _currencyNegativePattern;
        int      _currencyPositivePattern;
        String   _currencySymbol;
        String   _nanSymbol;
		    String   _negativeInfinitySymbol;
        String   _negativeSign;
        int      _numberDecimalDigits;
		    String   _numberDecimalSeparator;
		    String   _numberGroupSeparator;
		    IntArray _numberGroupSizes;
		    int      _numberNegativePattern;
        int      _percentDecimalDigits;
		    String   _percentDecimalSeparator;
		    String   _percentGroupSeparator;
		    IntArray _percentGroupSizes;
		    int      _percentNegativePattern;
		    int      _percentPositivePattern;
		    String   _percentSymbol;
        String   _perMilleSymbol;
		    String   _positiveInfinitySymbol;
		    String   _positiveSign;

      friend NumberFormatter;
      friend CultureInfo;
      };

    typedef AutoPtr<NumberFormatInfo> GCNumberFormatInfo;
    }
  }
