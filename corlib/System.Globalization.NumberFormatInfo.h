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
      private:
        bool     _isReadOnly;
        String   _decimalFormats;
        String   _currencyFormats;
        String   _percentFormats;
        String   _digitPattern;
        String   _zeroPatten;
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
        int32    _digitSubstitution;
      public:
        NumberFormatInfo();
        virtual ~NumberFormatInfo();
        virtual Object GetFormat() override;
        static NumberFormatInfo* GetInstance(IFormatProvider*);
        
        // Properties
        int32 CurrencyDecimalDigits();
        void CurrencyDecimalDigits(int32);
        String CurrencyDecimalSeparator();
        void CurrencyDecimalSeparator(String);
        String CurrencyGroupSeparator();
        void CurrencyGroupSeparator(String);
        IntArray CurrencyGroupSizes();
        void CurrencyGroupSizes(IntArray&);
        int32 CurrencyNegativePattern();
        void CurrencyNegativePattern(int32);
        int32 CurrencyPositivePattern();
        void CurrencyPositivePattern(int32);
        String CurrencySymbol();
        void CurrencySymbol(String);

        static NumberFormatInfo* CurrentInfo();
        
        bool IsReadOnly();

        String NaNSymbol();
        void NaNSymbol(String);
        String NegativeInfinitySymbol();
        void NegativeInfinitySymbol(String);
        String NegativeSign();
        void NegativeSign(String);
        int32 NumberDecimalDigits();
        void NumberDecimalDigits(int32);
        String NumberDecimalSeparator();
        void NumberDecimalSeparator(String);

        String PositiveInfinitySymbol();
        
      private:
        NumberFormatInfo(int, bool);
        IntArray& RawCurrencyGroupSizes();
        void RawCurrencyGroupSizes(IntArray&);

      friend NumberFormatter;
      friend CultureInfo;
      };

    typedef AutoPtr<NumberFormatInfo> GCNumberFormatInfo;
    }
  }
