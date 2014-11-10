#include "pch.h"
#include "System.Int64.h"
#include "System.Char.h"
#include "System.NumberFormatter.h"
#include "System.Threading.h"
#include "System.Math.h"

namespace System
  {
  Int64::Int64()
    :_value()
    {
    }

  Int64::Int64(int64 const& value)
    :_value(value)
    {
    }

  Int64::~Int64()
    {
    }

  Int64& Int64::operator = (int64 const& value)
    {
    _value = value;
    return *this;
    }

  Int64::operator int64 const& () const
    {
    return _value;
    }

  bool Int64::Equals(Object* obj)
    {
    Int64 i;
    if(!Object::IsInstance(*obj, i))
      return false;
    Int64* o = static_cast<Int64*>(obj);
    return (*o)._value == _value;
    }

  String Int64::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }

  String Int64::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }

  uint32 Int64::GetHashCode()
    {
    return abs((int32)(_value & 0xffffffff) ^ (int32)(_value >> 32));
    }

  int Int64::CompareTo(Object& value)
    {
    if(&value == nullptr)
      return 1;

    Int64 test;
    if(!(Object::IsInstance(test, value)))
      throw ArgumentException(L"Value is not a System.Int64");

    Int64& lValue = static_cast<Int64&>(value);

    if(_value == lValue)
      return 0;

    return (_value < lValue) ? -1 : 1;
    }

  bool Int64::TryParse(String s, Globalization::NumberStyles style, IFormatProvider* provider, int64& result)
    {
    GCException exc;
    if(!Parse(s, style, provider, true, result, exc)) 
      {
      result = 0;
      return false;
      }

    return true;
    }

  bool Int64::Parse(String s, Globalization::NumberStyles style, IFormatProvider* fp, bool tryParse, int64& result, GCException& exc)
    {
    using namespace Globalization;
    result = 0;
    exc.Reset();

    /*if(s == null)
      {
      if (!tryParse)
        exc = new ArgumentNullException ("s");
      return false;
      }*/

    if(s.Length() == 0) 
      {
      if(!tryParse)
        exc.Reset(new Exception(Int32::GetFormatException()));
      return false;
      }

    Globalization::NumberFormatInfo* nfi = nullptr;
    if(fp != nullptr) 
      {
      /*Type typeNFI = typeof(System.Globalization.NumberFormatInfo);
      nfi = (NumberFormatInfo) fp.GetFormat (typeNFI);*/
      } 
    if(nfi == nullptr)
      nfi = Threading::Thread::CurrentThread().CurrentCulture().NumberFormat();

    if(!Int32::CheckStyle(style, tryParse, exc))
      return false;

    bool AllowCurrencySymbol = ((intptr)style & (intptr)NumberStyles::AllowCurrencySymbol) != 0;
    bool AllowHexSpecifier = ((intptr)style & (intptr)NumberStyles::AllowHexSpecifier) != 0;
    bool AllowThousands = ((intptr)style & (intptr)NumberStyles::AllowThousands) != 0;
    bool AllowDecimalPoint = ((intptr)style & (intptr)NumberStyles::AllowDecimalPoint) != 0;
    bool AllowParentheses = ((intptr)style & (intptr)NumberStyles::AllowParentheses) != 0;
    bool AllowTrailingSign = ((intptr)style & (intptr)NumberStyles::AllowTrailingSign) != 0;
    bool AllowLeadingSign = ((intptr)style & (intptr)NumberStyles::AllowLeadingSign) != 0;
    bool AllowTrailingWhite = ((intptr)style & (intptr)NumberStyles::AllowTrailingWhite) != 0;
    bool AllowLeadingWhite = ((intptr)style & (intptr)NumberStyles::AllowLeadingWhite) != 0;
    bool AllowExponent = ((intptr)style & (intptr)NumberStyles::AllowExponent) != 0;

    int32 pos = 0;

    if(AllowLeadingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
      return false;

    bool foundOpenParentheses = false;
    bool negative = false;
    bool foundSign = false;
    bool foundCurrency = false;

    // Pre-number stuff
    if(AllowParentheses && s[pos] == L'(') 
      {
      foundOpenParentheses = true;
      foundSign = true;
      negative = true; // MS always make the number negative when there parentheses
      // even when NumberFormatInfo.NumberNegativePattern != 0!!!
      pos++;
      if(AllowLeadingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
        return false;

      if(s.Substring(pos, nfi->NegativeSign().Length()) == nfi->NegativeSign())
        {
        if(!tryParse)
          exc.Reset(new Exception(Int32::GetFormatException()));
        return false;
        }

      if(s.Substring(pos, nfi->PositiveSign().Length()) == nfi->PositiveSign()) 
        {
        if (!tryParse)
          exc.Reset(new Exception(Int32::GetFormatException()));
        return false;
        }
      }

    if(AllowLeadingSign && !foundSign)
      {
      // Sign + Currency
      Int32::FindSign(pos, s, nfi, foundSign, negative);
      if(foundSign)
        {
        if (AllowLeadingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
          return false;
        if(AllowCurrencySymbol) 
          {
          Int32::FindCurrency(pos, s, nfi, foundCurrency);
          if(foundCurrency && AllowLeadingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
            return false;
          }
        }
      }

    if(AllowCurrencySymbol && !foundCurrency) 
      {
      // Currency + sign
      Int32::FindCurrency(pos, s, nfi, foundCurrency);
      if(foundCurrency)
        {
        if(AllowLeadingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
          return false;
        if(foundCurrency) 
          {
          if(!foundSign && AllowLeadingSign) 
            {
            Int32::FindSign(pos, s, nfi, foundSign, negative);
            if(foundSign && AllowLeadingWhite && !Int32::JumpOverWhite (pos, s, true, tryParse, exc))
              return false;
            }
          }
        }
      }

    int64 number = 0;
    int32 nDigits = 0;
    int32 decimalPointPos = -1;
    int32 digitValue;
    wchar_t hexDigit;

    // Number stuff
    while(pos < s.Length()) 
      {

      if(!Int32::ValidDigit(s[pos], AllowHexSpecifier)) 
        {
        if(AllowThousands && (Int32::FindOther(pos, s, nfi->NumberGroupSeparator()) || Int32::FindOther(pos, s, nfi->CurrencyGroupSeparator())))
          continue;

        if(AllowDecimalPoint && decimalPointPos < 0 && (Int32::FindOther(pos, s, nfi->NumberDecimalSeparator()) || Int32::FindOther(pos, s, nfi->CurrencyDecimalSeparator()))) 
          {
          decimalPointPos = nDigits;
          continue;
          }

        break;
        }

      nDigits++;

      if(AllowHexSpecifier) 
        {
        hexDigit = s[pos++];
        if(Char::IsDigit(hexDigit))
          digitValue = (int32)(hexDigit - '0');
        else if (Char::IsLower(hexDigit))
          digitValue = (int32)(hexDigit - 'a' + 10);
        else
          digitValue = (int32)(hexDigit - 'A' + 10);

        ulong unumber = (ulong)number;

        // IMPROVME: We could avoid catching OverflowException
        try 
          {
          number = (int64)(unumber * 16ul + (uint64)digitValue);
          } 
        catch(Exception& ex)
          {
          if(!tryParse)
            exc.Reset(new Exception(ex));
          return false;
          }

        continue;
        }

      try 
        {
        // Calculations done as negative
        // (abs (MinValue) > abs (MaxValue))
        number = (number * 10 - (int64)(s[pos++] - L'0'));
        } 
      catch(Exception&)
        {
        if(!tryParse)
          //exc = new OverflowException("Value too large or too small.");
          exc.Reset(new SystemException(L"Value too large or too small."));
        return false;
        }				
      }

    // Post number stuff
    if(nDigits == 0)
      {
      if (!tryParse)
        exc.Reset(new Exception(Int32::GetFormatException()));
      return false;
      }

    int exponent = 0;
    if(AllowExponent)
      if(Int32::FindExponent(pos, s, exponent, tryParse, exc) && exc.Get() != nullptr)
        return false;

    if(AllowTrailingSign && !foundSign) 
      {
      // Sign + Currency
      Int32::FindSign(pos, s, nfi, foundSign, negative);
      if (foundSign && pos < s.Length()) 
        {
        if (AllowTrailingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
          return false;
        }
      }

    if(AllowCurrencySymbol && !foundCurrency) 
      {
      if(AllowTrailingWhite && pos < s.Length() && !Int32::JumpOverWhite(pos, s, false, tryParse, exc))
        return false;

      // Currency + sign
      Int32::FindCurrency (pos, s, nfi, foundCurrency);
      if(foundCurrency && pos < s.Length()) 
        {
        if (AllowTrailingWhite && !Int32::JumpOverWhite(pos, s, true, tryParse, exc))
          return false;
        if (!foundSign && AllowTrailingSign)
          Int32::FindSign(pos, s, nfi, foundSign, negative);
        }
      }

    if(AllowTrailingWhite && pos < s.Length() && !Int32::JumpOverWhite(pos, s, false, tryParse, exc))
      return false;

    if(foundOpenParentheses) 
      {
      if(pos >= s.Length() || s [pos++] != L')') 
        {
        if (!tryParse)
          exc.Reset(new Exception(Int32::GetFormatException()));
        return false;
        }
      if (AllowTrailingWhite && pos < s.Length() && !Int32::JumpOverWhite(pos, s, false, tryParse, exc))
        return false;
      }

    if(pos < s.Length() && s [pos] != 0x0000) 
      {
      if(!tryParse)
        exc.Reset(new Exception(Int32::GetFormatException()));
      return false;
      }

    if(!negative && !AllowHexSpecifier)
      {
      try 
        {
        number = -number;
        } 
      catch(Exception& ex)
        {
        if (!tryParse)
          exc.Reset(new Exception(ex));
        return false;
        }
      }

    if(decimalPointPos >= 0)
      exponent = exponent - nDigits + decimalPointPos;

    if(exponent < 0) 
      {
      //
      // Any non-zero values after decimal point are not allowed
      //
      int64 remainder;
      number = Math::DivRem(number, (int64)Math::Pow(10, -exponent), remainder);
      if(remainder != 0) 
        {
        if (!tryParse)
          //exc = new OverflowException ("Value too large or too small.");
          exc.Reset(new SystemException(L"Value too large or too small."));
        return false;
        }
      } 
    else if (exponent > 0) 
      {
      //
      // result *= 10^exponent
      //
      // Reduce the risk of throwing an overflow exc
      //
      double res = Math::Pow(10, exponent) * number;
      if (res < (double)MinValue || res > (double)MaxValue) 
        {
        if(!tryParse)
          //exc = new OverflowException ("Value too large or too small.");
          exc.Reset(new SystemException(L"Value too large or too small."));
        return false;
        }

      number = (long)res;
      }

    result = number;
    return true;
    }

  }
