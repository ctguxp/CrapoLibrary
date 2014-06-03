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
  String Int32::ToString(String format)
    {
    return NumberFormatter::NumberToString(&format, _value, nullptr);
    }
  String Int32::ToString(String format, IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(&format, _value, provider);
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
  }
