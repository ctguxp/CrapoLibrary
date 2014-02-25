// Perch Lake Computer System

#include "pch.h"
#include "System.Globalization.NumberFormatInfo.h"
#include "System.Exception.h"
#include "System.Threading.Thread.h"
#include "NumberFormatEntry.h"
#include "LocaleStrings.h"

namespace System
  {
  namespace Globalization
    {
    NumberFormatInfo::NumberFormatInfo()
      :_isReadOnly(false)
      ,_currencyDecimalDigits(2)
      ,_currencyDecimalSeparator(L".")
      ,_currencyGroupSeparator(L",")
      ,_currencyGroupSizes(1)
      ,_currencyNegativePattern(0)
      ,_currencyPositivePattern(0)
      ,_currencySymbol(L"\u00a4")
      ,_nanSymbol(L"NaN")
      ,_negativeInfinitySymbol(L"-Infinity")
      ,_negativeSign(L"-")
      ,_numberDecimalDigits(2)
		  ,_numberDecimalSeparator(L".")
		  ,_numberGroupSeparator(L",")
		  ,_numberGroupSizes(1)
		  ,_numberNegativePattern(1)
      ,_percentDecimalDigits(2)
		  ,_percentDecimalSeparator(L".")
		  ,_percentGroupSeparator(L",")
		  ,_percentGroupSizes(1)
		  ,_percentNegativePattern(0)
		  ,_percentPositivePattern(0)
		  ,_percentSymbol(L"%")
      ,_perMilleSymbol(L"\u2030")
		  ,_positiveInfinitySymbol(L"Infinity")
		  ,_positiveSign(L"+")
      {
      _currencyGroupSizes[0] = 3;
      _numberGroupSizes[0] = 3;
      _percentGroupSizes[0] = 3;
      }
    NumberFormatInfo::NumberFormatInfo(int index, bool read_only)
      :_isReadOnly(read_only)
      ,_currencyDecimalDigits(2)
      ,_currencyDecimalSeparator(L".")
      ,_currencyGroupSeparator(L",")
      ,_currencyGroupSizes(1)
      ,_currencyNegativePattern(0)
      ,_currencyPositivePattern(0)
      ,_currencySymbol(L"\u00a4")
      ,_nanSymbol(L"NaN")
      ,_negativeInfinitySymbol(L"-Infinity")
      ,_negativeSign(L"-")
      ,_numberDecimalDigits(2)
		  ,_numberDecimalSeparator(L".")
		  ,_numberGroupSeparator(L",")
		  ,_numberGroupSizes(1)
		  ,_numberNegativePattern(1)
      ,_percentDecimalSeparator(L".")
		  ,_percentGroupSeparator(L",")
		  ,_percentGroupSizes(1)
		  ,_percentNegativePattern(0)
		  ,_percentPositivePattern(0)
		  ,_percentSymbol(L"%")
      ,_perMilleSymbol(L"\u2030")
		  ,_positiveInfinitySymbol(L"Infinity")
		  ,_positiveSign(L"+")
      {
      _currencyGroupSizes[0] = 3;
      _numberGroupSizes[0] = 3;
      _percentGroupSizes[0] = 3;

      const NumberFormatEntry *nfe = &number_format_entries[index];

      _currencyDecimalDigits = nfe->currency_decimal_digits;
      _currencyDecimalSeparator = MakeLocaleString(nfe->currency_decimal_separator);
      _currencyGroupSeparator = MakeLocaleString(nfe->currency_group_separator);
      _currencyGroupSizes = MakeLocaleArray(nfe->currency_group_sizes, GROUP_SIZE);
      _currencyNegativePattern = nfe->currency_negative_pattern;
      _currencyPositivePattern = nfe->currency_positive_pattern;
      _currencySymbol = MakeLocaleString(nfe->currency_symbol);  
      _nanSymbol = MakeLocaleString(nfe->nan_symbol);
      _negativeInfinitySymbol = MakeLocaleString(nfe->negative_infinity_symbol);
      _negativeSign = MakeLocaleString(nfe->negative_sign);
      _numberDecimalDigits = nfe->number_decimal_digits;
		  _numberDecimalSeparator = MakeLocaleString(nfe->number_decimal_separator);
		  _numberGroupSeparator = MakeLocaleString(nfe->number_group_separator);
		  _numberGroupSizes = MakeLocaleArray(nfe->number_group_sizes, GROUP_SIZE);
		  _numberNegativePattern = nfe->number_negative_pattern;
      _percentDecimalSeparator = MakeLocaleString(nfe->percent_decimal_separator);
		  _percentGroupSeparator = MakeLocaleString(nfe->percent_group_separator);
		  _percentGroupSizes = MakeLocaleArray(nfe->percent_group_sizes, GROUP_SIZE);
		  _percentNegativePattern = nfe->percent_negative_pattern;
		  _percentPositivePattern = nfe->percent_positive_pattern;
		  _percentSymbol = MakeLocaleString(nfe->percent_symbol);
      _perMilleSymbol = MakeLocaleString(nfe->per_mille_symbol);
		  _positiveInfinitySymbol = MakeLocaleString(nfe->positive_infinity_symbol);
		  _positiveSign = MakeLocaleString(nfe->positive_sign);
      }
    NumberFormatInfo::~NumberFormatInfo()
      {
      }
    Object NumberFormatInfo::GetFormat()
      {
      throw SystemException(L"Not Implemented");
      }
    NumberFormatInfo* NumberFormatInfo::GetInstance(IFormatProvider* formatProvider)
      {
      if(formatProvider != nullptr)
        {
        /* TODO: NumberFormatInfo nfi;
        nfi = (NumberFormatInfo)formatProvider->GetFormat(typeof(NumberFormatInfo));
        if (nfi != null)
          return nfi;*/
        }

      return CurrentInfo();
      }
    
    NumberFormatInfo* NumberFormatInfo::CurrentInfo()
      {
      Threading::Thread& current = Threading::Thread::CurrentThread();
      NumberFormatInfo* nfi = current.CurrentCulture().NumberFormat();
			//nfi->isReadOnly = true;
			return nfi;     
		  }
    String NumberFormatInfo::CurrencyDecimalSeparator()
      {
      return _currencyDecimalSeparator;
      }
    String NumberFormatInfo::CurrencyGroupSeparator()
      {
      return _currencyGroupSeparator;
      }
    String NumberFormatInfo::CurrencySymbol()
      {
      return _currencySymbol;
      }
    void NumberFormatInfo::CurrencySymbol(cstring value)
      {
      _currencySymbol = value;
      }
    String NumberFormatInfo::NegativeSign()
      {
      return _negativeSign;
      }
    }
  }
