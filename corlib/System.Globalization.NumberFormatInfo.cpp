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
      ,_decimalFormats()
      ,_currencyFormats()
      ,_percentFormats()
      ,_digitPattern(L"#")
      ,_zeroPatten(L"0")
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
      ,_digitSubstitution(1)
      {
      _currencyGroupSizes[0] = 3;
      _numberGroupSizes[0] = 3;
      _percentGroupSizes[0] = 3;
      }
    NumberFormatInfo::NumberFormatInfo(int index, bool read_only)
      :_isReadOnly(read_only)
      ,_decimalFormats()
      ,_currencyFormats()
      ,_percentFormats()
      ,_digitPattern(L"#")
      ,_zeroPatten(L"0")
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
      ,_digitSubstitution(1)
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

    // Properties

    // ------------------------------------------------------------------------
    /// Gets or sets the number of decimal places to use in currency values.
    int32 NumberFormatInfo::CurrencyDecimalDigits()
      { 
      return _currencyDecimalDigits;
      }
    void NumberFormatInfo::CurrencyDecimalDigits(int32 value)
      {
      if(value < 0 || value > 99)
        {
        throw ArgumentOutOfRangeException(L"The value specified for the property is less than 0 or greater than 99");
        }
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencyDecimalDigits = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the decimal separator in currency values.
    String NumberFormatInfo::CurrencyDecimalSeparator()
      {
      return _currencyDecimalSeparator;
      }
    void NumberFormatInfo::CurrencyDecimalSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencyDecimalSeparator = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that separates groups of digits to the left of the decimal in currency values.
    String NumberFormatInfo::CurrencyGroupSeparator()
      {
      return _currencyGroupSeparator;
      }
    void NumberFormatInfo::CurrencyGroupSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencyGroupSeparator = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the number of digits in each group to the left of the decimal in currency values.
    IntArray NumberFormatInfo::CurrencyGroupSizes()
      {
      return RawCurrencyGroupSizes();
      }
    void NumberFormatInfo::CurrencyGroupSizes(IntArray& value)
      {
      RawCurrencyGroupSizes(value);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the format pattern for negative currency values.
    int32 NumberFormatInfo::CurrencyNegativePattern()
      { 
      return _currencyNegativePattern;
      }
    void NumberFormatInfo::CurrencyNegativePattern(int32 value)
      {
      if(value < 0 || value > 15)
        throw ArgumentOutOfRangeException(L"The value specified for the property is less than 0 or greater than 15");
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencyNegativePattern = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the format pattern for positive currency values.
    int32 NumberFormatInfo::CurrencyPositivePattern()
      { 
      return _currencyPositivePattern;
      }
    void NumberFormatInfo::CurrencyPositivePattern(int32 value)
      {
      if(value < 0 || value > 3)
        throw ArgumentOutOfRangeException(L"The value specified for the property is less than 0 or greater than 3");
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencyPositivePattern = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the currency symbol.
    String NumberFormatInfo::CurrencySymbol()
      {
      return _currencySymbol;
      }
    void NumberFormatInfo::CurrencySymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _currencySymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets a read-only NumberFormatInfo that formats values based on the current culture.
    NumberFormatInfo* NumberFormatInfo::CurrentInfo()
      {
      Threading::Thread& current = Threading::Thread::CurrentThread();
      NumberFormatInfo* nfi = current.CurrentCulture().NumberFormat();
      nfi->_isReadOnly = true;
      return nfi;     
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets a value indicating whether the NumberFormatInfo is read-only.
    bool NumberFormatInfo::IsReadOnly()
      {
      return _isReadOnly;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that represents the IEEE NaN (not a number) value.
    String NumberFormatInfo::NaNSymbol()
      {
      return _nanSymbol;
      }
    void NumberFormatInfo::NaNSymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _nanSymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that represents negative infinity.
    String NumberFormatInfo::NegativeInfinitySymbol()
      {
      return _negativeInfinitySymbol;
      }
    void NumberFormatInfo::NegativeInfinitySymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _negativeInfinitySymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that denotes that the associated number is negative.
    String NumberFormatInfo::NegativeSign()
      {
      return _negativeSign;
      }
    void NumberFormatInfo::NegativeSign(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _negativeSign = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the number of decimal places to use in numeric values.
    int32 NumberFormatInfo::NumberDecimalDigits()
      {
      return _numberDecimalDigits;
      }
    void NumberFormatInfo::NumberDecimalDigits(int32 value)
      {
      if (value < 0 || value > 99)
        throw ArgumentOutOfRangeException(L"The value specified for the property is less than 0 or greater than 99");
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _numberDecimalDigits = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the decimal separator in numeric values.
    String NumberFormatInfo::NumberDecimalSeparator()
      {
      return _numberDecimalSeparator;
      }
    void NumberFormatInfo::NumberDecimalSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _numberDecimalSeparator = value;
      }
    // ------------------------------------------------------------------------





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

    String NumberFormatInfo::PositiveInfinitySymbol()
      {
      return _positiveInfinitySymbol;
      }


    IntArray& NumberFormatInfo::RawCurrencyGroupSizes()
      { 
      return _currencyGroupSizes;
      }
    void NumberFormatInfo::RawCurrencyGroupSizes(IntArray& value)
      {
      if(value.IsNull())
        throw ArgumentNullException(L"The value specified for the property is a null reference");
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      if(value.Length() == 0)
        {
        _currencyGroupSizes.Length(0);
        return;
        }
      sizet num = value.Length() - 1;
      for(sizet i = 0; i < num; i++)
        {
        if(value[i] < 1 || value[i] > 9)
          {
          throw ArgumentOutOfRangeException(L"One of the elements in the array specified is not between 1 and 9");
          }
        }
      if(value[num] < 0 || value[num] > 9)
        {
        throw ArgumentOutOfRangeException(L"Last element in the array specified is not between 0 and 9");
        }
      _currencyGroupSizes = value;
      } 

    }
  }
