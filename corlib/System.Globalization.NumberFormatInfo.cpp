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
    NumberFormatInfo::NumberFormatInfo(bool read_only)
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

    NumberFormatInfo::NumberFormatInfo(const NumberFormatInfo& nfi)
      :_isReadOnly(nfi._isReadOnly)
      ,_decimalFormats(nfi._decimalFormats)
      ,_currencyFormats(nfi._currencyFormats)
      ,_percentFormats(nfi._percentFormats)
      ,_digitPattern(nfi._digitPattern)
      ,_zeroPatten(nfi._zeroPatten)
      ,_currencyDecimalDigits(nfi._currencyDecimalDigits)
      ,_currencyDecimalSeparator(nfi._currencyDecimalSeparator)
      ,_currencyGroupSeparator(nfi._currencyGroupSeparator)
      ,_currencyGroupSizes(nfi._currencyGroupSizes.Length())
      ,_currencyNegativePattern(nfi._currencyNegativePattern)
      ,_currencyPositivePattern(nfi._currencyPositivePattern)
      ,_currencySymbol(nfi._currencySymbol)
      ,_nanSymbol(nfi._nanSymbol)
      ,_negativeInfinitySymbol(nfi._negativeInfinitySymbol)
      ,_negativeSign(nfi._negativeSign)
      ,_numberDecimalDigits(nfi._numberDecimalDigits)
      ,_numberDecimalSeparator(nfi._numberDecimalSeparator)
      ,_numberGroupSeparator(nfi._numberGroupSeparator)
      ,_numberGroupSizes(nfi._numberGroupSizes.Length())
      ,_numberNegativePattern(nfi._numberNegativePattern)
      ,_percentDecimalDigits(nfi._percentDecimalDigits)
      ,_percentDecimalSeparator(nfi._percentDecimalSeparator)
      ,_percentGroupSeparator(nfi._percentGroupSeparator)
      ,_percentGroupSizes(nfi._percentGroupSizes.Length())
      ,_percentNegativePattern(nfi._percentNegativePattern)
      ,_percentPositivePattern(nfi._percentPositivePattern)
      ,_percentSymbol(nfi._percentSymbol)
      ,_perMilleSymbol(nfi._perMilleSymbol)
      ,_positiveInfinitySymbol(nfi._positiveInfinitySymbol)
      ,_positiveSign(nfi._positiveSign)
      ,_digitSubstitution(nfi._digitSubstitution)
      {
      _currencyGroupSizes = nfi._currencyGroupSizes;
      _numberGroupSizes = nfi._numberGroupSizes;
      _percentGroupSizes = nfi._percentGroupSizes;
      }

    NumberFormatInfo& NumberFormatInfo::operator=(const NumberFormatInfo& nfi)
      {
      if(this == &nfi)
        return *this;

      _isReadOnly = nfi._isReadOnly;
      _decimalFormats = nfi._decimalFormats;
      _currencyFormats = nfi._currencyFormats;
      _percentFormats = nfi._percentFormats;
      _digitPattern = nfi._digitPattern;
      _zeroPatten = nfi._zeroPatten;
      _currencyDecimalDigits = nfi._currencyDecimalDigits;
      _currencyDecimalSeparator = nfi._currencyDecimalSeparator;
      _currencyGroupSeparator = nfi._currencyGroupSeparator;
      _currencyGroupSizes = nfi._currencyGroupSizes.Length();
      _currencyNegativePattern = nfi._currencyNegativePattern;
      _currencyPositivePattern = nfi._currencyPositivePattern;
      _currencySymbol = nfi._currencySymbol;
      _nanSymbol = nfi._nanSymbol;
      _negativeInfinitySymbol = nfi._negativeInfinitySymbol;
      _negativeSign = nfi._negativeSign;
      _numberDecimalDigits = nfi._numberDecimalDigits;
      _numberDecimalSeparator = nfi._numberDecimalSeparator;
      _numberGroupSeparator = nfi._numberGroupSeparator;
      _numberGroupSizes = nfi._numberGroupSizes.Length();
      _numberNegativePattern = nfi._numberNegativePattern;
      _percentDecimalDigits = nfi._percentDecimalDigits;
      _percentDecimalSeparator = nfi._percentDecimalSeparator;
      _percentGroupSeparator = nfi._percentGroupSeparator;
      _percentGroupSizes = nfi._percentGroupSizes.Length();
      _percentNegativePattern = nfi._percentNegativePattern;
      _percentPositivePattern = nfi._percentPositivePattern;
      _percentSymbol = nfi._percentSymbol;
      _perMilleSymbol = nfi._perMilleSymbol;
      _positiveInfinitySymbol = nfi._positiveInfinitySymbol;
      _positiveSign = nfi._positiveSign;
      _digitSubstitution = nfi._digitSubstitution;

      _currencyGroupSizes = nfi._currencyGroupSizes;
      _numberGroupSizes = nfi._numberGroupSizes;
      _percentGroupSizes = nfi._percentGroupSizes;

      return *this;
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
    Object* NumberFormatInfo::GetFormat(Object*)
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

    /// Gets the default read-only NumberFormatInfo that is culture-independent (invariant).
    NumberFormatInfo* NumberFormatInfo::InvariantInfo()
      {
      // This uses invariant info, which is same as in the constructor
      return new NumberFormatInfo(true);
      }

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

    // ------------------------------------------------------------------------
    /// Gets or sets the string that separates groups of digits to the left of the decimal in numeric values
    String NumberFormatInfo::NumberGroupSeparator()
      {
      return _numberGroupSeparator;
      }
    void NumberFormatInfo::NumberGroupSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _numberGroupSeparator = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the number of digits in each group to the left of the decimal in numeric values.
    IntArray NumberFormatInfo::NumberGroupSizes()
      {
      return RawNumberGroupSizes();
      }
    void NumberFormatInfo::NumberGroupSizes(IntArray& value)
      {
      RawNumberGroupSizes(value);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the format pattern for negative numeric values.
    int NumberFormatInfo::NumberNegativePattern()
      {
      return _numberNegativePattern;
      }
    void NumberFormatInfo::NumberNegativePattern(int value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _numberNegativePattern = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the number of decimal places to use in percent values.
    int NumberFormatInfo::PercentDecimalDigits()
      {
      return _percentDecimalDigits;
      }
    void NumberFormatInfo::PercentDecimalDigits(int value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentDecimalDigits = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the decimal separator in percent values. 
    String NumberFormatInfo::PercentDecimalSeparator()
      {
      return _percentDecimalSeparator;
      }
    void NumberFormatInfo::PercentDecimalSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentDecimalSeparator = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that separates groups of digits to the left of the decimal in percent values.
    String NumberFormatInfo::PercentGroupSeparator()
      {
      return _percentGroupSeparator;
      }
    void NumberFormatInfo::PercentGroupSeparator(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentGroupSeparator = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the number of digits in each group to the left of the decimal in percent values. 
    IntArray NumberFormatInfo::PercentGroupSizes()
      {
      return RawPercentGroupSizes();
      }
    void NumberFormatInfo::PercentGroupSizes(IntArray& value)
      {
      RawPercentGroupSizes(value);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the format pattern for negative percent values.
    int NumberFormatInfo::PercentNegativePattern()
      {
      return _percentNegativePattern;
      }
    void NumberFormatInfo::PercentNegativePattern(int value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentNegativePattern = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the format pattern for positive percent values.
    int NumberFormatInfo::PercentPositivePattern()
      {
      return _percentPositivePattern;
      }
    void NumberFormatInfo::PercentPositivePattern(int value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentPositivePattern = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the percent symbol.
    String NumberFormatInfo::PercentSymbol()
      {
      return _percentSymbol;
      }
    void NumberFormatInfo::PercentSymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _percentSymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string to use as the per mille symbol.
    String NumberFormatInfo::PerMilleSymbol()
      {
      return _perMilleSymbol;
      }
    void NumberFormatInfo::PerMilleSymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _perMilleSymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that represents positive infinity.
    String NumberFormatInfo::PositiveInfinitySymbol()
      {
      return _positiveInfinitySymbol;
      }
    void NumberFormatInfo::PositiveInfinitySymbol(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _positiveInfinitySymbol = value;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets or sets the string that denotes that the associated number is positive.
    String NumberFormatInfo::PositiveSign()
      {
      return _positiveSign;
      }
    void NumberFormatInfo::PositiveSign(String value)
      {
      if(_isReadOnly)
        {
        throw SystemException(L"The current instance is read-only and a set operation was attempted");  
        // TODO : throw InvalidOperationException("The current instance is read-only and a set operation was attempted");
        }
      _positiveSign = value;
      }
    // ------------------------------------------------------------------------


    NumberFormatInfo* NumberFormatInfo::GetInstance(IFormatProvider* formatProvider)
      {
      if(formatProvider != nullptr)
        {
        NumberFormatInfo type;
        NumberFormatInfo* nfi = (NumberFormatInfo*)formatProvider->GetFormat(&type);
        return nfi;
        /* TODO: NumberFormatInfo nfi;
        nfi = (NumberFormatInfo)formatProvider->GetFormat(typeof(NumberFormatInfo));
        if (nfi != null)
        return nfi;*/
        }

      return CurrentInfo();
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

    IntArray& NumberFormatInfo::RawNumberGroupSizes()
      { 
      return _numberGroupSizes;
      }
    void NumberFormatInfo::RawNumberGroupSizes(IntArray& value)
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
        _numberGroupSizes.Length(0);
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
      _numberGroupSizes = value;
      }

    IntArray& NumberFormatInfo::RawPercentGroupSizes()
      { 
      return _percentGroupSizes;
      }
    void NumberFormatInfo::RawPercentGroupSizes(IntArray& value)
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
        _percentGroupSizes.Length(0);
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
      _percentGroupSizes = value;
      }  

    }
  }
