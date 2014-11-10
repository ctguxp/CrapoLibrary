// Perch Lake Computer System

#include "pch.h"
#include "System.Int32.h"
#include "System.Char.h"
#include "System.NumberFormatter.h"
#include "System.FormatException.h"

namespace System
  {
  Int32::Int32()
    :_value()
    {
    }
  Int32::Int32(int32 const& value)
    :_value(value)
    {
    }
  Int32::~Int32()
    {
    }
  Int32& Int32::operator = (int32 const& value)
    {
    _value = value;
    return *this;
    }
  Int32::operator int32 const& () const
    {
    return _value;
    }
  String Int32::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Int32::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }
  String Int32::ToString(String format)
    {
    return NumberFormatter::NumberToString(&format, _value, nullptr);
    }
  String Int32::ToString(String format, IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(&format, _value, provider);
    }
  bool Int32::Equals(Object* obj)
    {
    Int32 i;
    if(!Object::IsInstance(*obj, i))
      return false;
    Int32* o = static_cast<Int32*>(obj);
    return (*o)._value == _value;
    }
  uint32 Int32::GetHashCode()
    {
    return (uint32)abs(_value);
    }
  Exception Int32::GetFormatException()
    {
    return FormatException(L"Input string was not in the correct format");
    }
  int Int32::Parse(String s)
    {
    Exception exc;
    int res;

    if(!Parse(s, false, res, exc))
      throw exc;

    return res;
    }
  bool Int32::ProcessTrailingWhitespace(bool tryParse, String s, int position, Exception& exc)
    {
    int len = s.Length();

    for(int i = position; i < len; i++)
      {
      wchar_t c = s [i];

      if(c != 0 && !Char::IsWhiteSpace(c)){
        if (!tryParse)
          exc = GetFormatException();
        return false;
        }
      }
    return true;
    }
  bool Int32::Parse(String s, bool tryParse, int& result, Exception& exc)
    {
    int val = 0;
    int i, sign = 1;
    bool digits_seen = false;

    result = 0;
    Globalization::NumberFormatInfo* nfi = Threading::Thread::CurrentThread().CurrentCulture().NumberFormat();

    int len = s.Length();

    wchar_t c;
    for(i = 0; i < len; i++)
      {
      c = s[i];
      if (!Char::IsWhiteSpace(c))
        break;
      }

    if(i == len) 
      {
      if(!tryParse)
        exc = GetFormatException();
      return false;
      }

    auto ps_length = nfi->PositiveSign().Length();
    auto ns_length = nfi->NegativeSign().Length();
    String pos = nfi->PositiveSign();
    String neg = nfi->NegativeSign();
    if (len > ps_length && String::CompareOrdinalUnchecked(s, i, ns_length, pos, 0, ps_length) == 0)
      i += ps_length;
    else if (len > ns_length && String::CompareOrdinalUnchecked(s, i, ns_length, neg, 0, ns_length) == 0)
      {
      sign = -1;
      i += ns_length;
      }

    for(; i < len; i++)
      {
      c = s [i];

      if(c == L'\0') 
        {
        i = len;
        continue;
        }

      if(c >= L'0' && c <= L'9')
        {
        byte d = (byte)(c - '0');

        if(val > (MaxValue/10))
          {
          if (!tryParse)
            //exc = OverflowException ("Value is too large");
              exc.Message(L"Value is too large");
          return false;
          }

        if(val == (MaxValue/10))
          {
          if((d > (MaxValue % 10)) && (sign == 1 || (d > ((MaxValue % 10) + 1))))
            {
            if (!tryParse)
              //exc = new OverflowException ("Value is too large");
                exc.Message(L"Value is too large");
            return false;
            }
          if (sign == -1)
            val = (val * sign * 10) - d;
          else
            val = (val * 10) + d;

          if(ProcessTrailingWhitespace(tryParse, s, i + 1, exc))
            {
            result = val;
            return true;
            }
          if(!tryParse)
            //exc = OverflowException(L"Value is too large");
              exc.Message(L"Value is too large");
          return false;
          } 
        else 
          val = val * 10 + d;

        digits_seen = true;
        } 
      else if(!ProcessTrailingWhitespace(tryParse, s, i, exc))
        return false;
      }
    if(!digits_seen) 
      {
      if(!tryParse)
        exc = GetFormatException();
      return false;
      }

    if(sign == -1)
      result = val * sign;
    else
      result = val;

    return true;
    }

  int Int32::CompareTo(Object& value)
    {
    if(&value == nullptr)
      return 1;

    Int32 test;
    if(!(Object::IsInstance(test, value)))
      throw ArgumentException(L"Value is not a System.Int32");

    Int32& xv = static_cast<Int32&>(value);
    if(_value == xv)
      return 0;
    if(_value > xv)
      return 1;
    else
      return -1;
    }

  bool Int32::JumpOverWhite(int32& pos, String s, bool reportError, bool tryParse, GCException& exc)
    {
    while(pos < s.Length() && Char::IsWhiteSpace(s[pos]))
      pos++;

    if(reportError && pos >= s.Length()) 
      {
      if(!tryParse)
        exc.Reset(new Exception(GetFormatException()));
      return false;
      }

    return true;
    }

  bool Int32::CheckStyle(Globalization::NumberStyles style, bool tryParse, GCException& exc)
    {
    using namespace Globalization;
    if(((intptr)style & (intptr)NumberStyles::AllowHexSpecifier) != 0)
      {
      NumberStyles ne = (NumberStyles)((intptr)style ^ (intptr)NumberStyles::AllowHexSpecifier);
      if(((intptr)ne & (intptr)NumberStyles::AllowLeadingWhite) != 0)
        ne = (NumberStyles)((intptr)ne ^ (intptr)NumberStyles::AllowLeadingWhite);
      if(((intptr)ne & (intptr)NumberStyles::AllowTrailingWhite) != 0)
        ne = (NumberStyles)((intptr)ne ^ (intptr)NumberStyles::AllowTrailingWhite);
      if(ne != NumberStyles::None) 
        {
        if(!tryParse)
          exc.Reset(new ArgumentException(L"With AllowHexSpecifier only AllowLeadingWhite and AllowTrailingWhite are permitted."));
        return false;
        }
      } 
    else if((uintptr) style > (uintptr) NumberStyles::Any)
      {
      if(!tryParse)
        exc.Reset(new ArgumentException(L"Not a valid number style"));
      return false;
      }

    return true;
    }

  void Int32::FindSign(int32& pos, String s, Globalization::NumberFormatInfo* nfi, bool& foundSign, bool& negative)
    {
    if((pos + nfi->NegativeSign().Length()) <= s.Length() && s.IndexOfOrdinalUnchecked(nfi->NegativeSign(), pos, nfi->NegativeSign().Length()) == pos) 
      {
      negative = true;
      foundSign = true;
      pos += nfi->NegativeSign().Length();
      } 
    else if((pos + nfi->PositiveSign().Length()) <= s.Length() && s.IndexOfOrdinalUnchecked(nfi->PositiveSign(), pos, nfi->PositiveSign().Length()) == pos) 
      {
      negative = false;
      pos += nfi->PositiveSign().Length();
      foundSign = true;
      } 
    }

  void Int32::FindCurrency(int32& pos, String s, Globalization::NumberFormatInfo* nfi, bool& foundCurrency)
    {
    if((pos + nfi->CurrencySymbol().Length()) <= s.Length() && s.Substring(pos, nfi->CurrencySymbol().Length()) == nfi->CurrencySymbol())
      {
      foundCurrency = true;
      pos += nfi->CurrencySymbol().Length();
      } 
    }

  bool Int32::FindOther(int32 pos, String s, String other)
    {
    if((pos + other.Length()) <= s.Length() && s.Substring(pos, other.Length()) == other) 
      {
      pos += other.Length();
      return true;
      } 

    return false;
    }

  bool Int32::ValidDigit(wchar_t e, bool allowHex)
    {
    if(allowHex)
      return Char::IsDigit(e) || (e >= L'A' && e <= L'F') || (e >= L'a' && e <= L'f');

    return Char::IsDigit(e);
    }

  bool Int32::FindExponent(int32 pos, String s, int32 exponent, bool tryParse, GCException& exc)
    {
    exponent = 0;

    if(pos >= s.Length() || (s[pos] != L'e' && s[pos] != L'E')) 
      {
      exc.Reset();
      return false;
      }

    auto i = pos + 1;
    if(i == s.Length()) 
      {
      exc.Reset(tryParse ? nullptr : new Exception(GetFormatException()));
      return true;
      }

    // negative exponent not valid for Int32
    if(s[i] == L'-') 
      {
      //exc = tryParse ? null : new OverflowException ("Value too large or too small.");
      exc.Reset(tryParse ? nullptr : new SystemException(L"Value too large or too small."));
      return true;
      }

    if(s[i] == L'+' && ++i == s.Length()) 
      {
      exc.Reset(tryParse ? nullptr : new Exception(GetFormatException()));
      return true;
      }

    int64 exp = 0; // temp long value
    for(; i < s.Length(); i++) 
      {
      if(!Char::IsDigit(s[i]))  
        {
        exc.Reset(tryParse ? nullptr : new Exception(GetFormatException()));
        return true;
        }

      // Reduce the risk of throwing an overflow exc
      exp = exp * 10 - (int32)(s [i] - L'0');
      if(exp < Int32::MinValue || exp > Int32::MaxValue) 
        {
        //exc = tryParse ? nullptr : new OverflowException ("Value too large or too small.");
        exc.Reset(tryParse ? nullptr : new SystemException(L"Value too large or too small."));
        return true;
        }
      }

    // exp value saved as negative
    exp = -exp;

    exc.Reset();
    exponent = (int32)exp;
    pos = i;
    return true;
    }

  }
