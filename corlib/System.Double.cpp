#include "pch.h"
#include "System.Double.h"
#include "System.NumberFormatter.h"
#include "System.Char.h"
#include "System.FormatException.h"
#include "System.Array.hpp"
#include <limits>

namespace System
  {
  const double Double::NaN = std::numeric_limits<double>::signaling_NaN();
  const double Double::MaxValue = DBL_MAX;
  const double Double::MinValue = DBL_MIN;
  const double Double::NegativeInfinity = -std::numeric_limits<double>::infinity();
  const double Double::PositiveInfinity = std::numeric_limits<double>::infinity();

  Double::Double()
    :_value()
    {
    }
  Double::Double(double const& value)
    :_value(value)
    {
    }
  Double::~Double()
    {
    }
  Double& Double::operator = (double const& value)
    {
    _value = value;
    return *this;
    }
  Double::operator double const& () const
    {
    return _value;
    }
  uint32 Double::GetHashCode()
    {
    if(_value == 0)
      return 0;
    int exp;
    double mantissa = frexp(_value, &exp);
    return (uint32)(2 * fabs(mantissa) - 1) * ~0U;
    }
  String Double::ToString()
    {
    return NumberFormatter::NumberToString(_value, nullptr);
    }
  String Double::ToString(IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(_value, provider);
    }
  String Double::ToString(String format)
    {
    return ToString(format, nullptr);
    }
  String Double::ToString(String format, IFormatProvider* provider)
    {
    return NumberFormatter::NumberToString(&format, _value, provider);
    }

  bool Double::IsNegativeInfinity(double d)
    {
    return (d < 0.0 && (d == NegativeInfinity || d == PositiveInfinity));
    }

  bool Double::IsPositiveInfinity(double d)
    {
    return (d > 0.0 && (d == NegativeInfinity || d == PositiveInfinity));
    }

  double Double::Parse(String s, IFormatProvider* provider)
    {
    using namespace Globalization;
    return Parse(s, (NumberStyles)((intptr)NumberStyles::Float | (intptr)NumberStyles::AllowThousands), provider);
    }

  double Double::Parse(String s, Globalization::NumberStyles style, IFormatProvider* provider)
    {
    Exception exc;
    double result;

    if(!Parse(s, style, provider, false, result, exc))
      throw exc;

    return result;
    }

  // FIXME: check if digits are group in correct numbers between the group separators
  bool Double::Parse(String s, Globalization::NumberStyles styleIn, IFormatProvider* provider, bool tryParse, double& result, Exception& exc)
    {
    using namespace Globalization;
    ParseStruct ps;
    ::ZeroMemory(&ps, sizeof(ParseStruct));
    ps.tryParse = tryParse;
    ps.style = styleIn;
    result = 0;

    if(s.Length() == 0)
      {
      if(!tryParse)
        exc = FormatException();
      return false;
      }
    // yes it's counter intuitive (buggy?) but even TryParse actually throws in this case
    if(((intptr)styleIn & (intptr)NumberStyles::AllowHexSpecifier) != 0)
      {
      String ahs("AllowHexSpecifier");
      String msg = String::Format(L"Double doesn't support parsing with '{0}'.", &ahs);
      throw ArgumentException(msg);
      }
    if(styleIn > NumberStyles::Any) 
      {
      if(!tryParse)
        exc = ArgumentException();
      return false;
      }

    NumberFormatInfo* format = NumberFormatInfo::GetInstance(provider);
    if(format == nullptr) throw Exception(L"How did this happen?");

    //
    // validate and prepare string for C
    //
    int len = s.Length();
    bool allow_leading_white = ((intptr)styleIn & (intptr)NumberStyles::AllowLeadingWhite) != 0;
    ps.allow_trailing_white = (((intptr)styleIn & (intptr)NumberStyles::AllowTrailingWhite) != 0);

    if(allow_leading_white) 
      {
      while(ps.sidx < len && Char::IsWhiteSpace(s[ps.sidx]))
        ps.sidx++;

      if(ps.sidx == len) 
        {
        if(!tryParse)
          exc = Int32::GetFormatException();
        return false;
        }
      }
    int sEndPos = s.Length() - 1;
    if(ps.allow_trailing_white)
      while(Char::IsWhiteSpace(s [sEndPos]))
        sEndPos--;

    if(TryParseStringConstant(format->NaNSymbol(), s, ps.sidx, sEndPos)) 
      {
      result = Double::NaN;
      return true;
      }
    if(TryParseStringConstant(format->PositiveInfinitySymbol(), s, ps.sidx, sEndPos))
      {
      result = Double::PositiveInfinity;
      return true;
      }
    if (TryParseStringConstant(format->NegativeInfinitySymbol(), s, ps.sidx, sEndPos))
      {
      result = Double::NegativeInfinity;
      return true;
      }

    ByteArray b(len + 1);
    ps.b = b.ToPtr();

    //
    // Machine state
    //
    ps.state = ParseState::AllowSign;

    //
    // Setup
    //
    String decimal_separator;
    String group_separator;
    String currency_symbol;
    int decimal_separator_len = 0;
    int group_separator_len = 0;
    ps.currency_symbol_len = 0;
    if(((intptr)styleIn & (intptr)NumberStyles::AllowDecimalPoint) != 0)
      {
      decimal_separator = format->NumberDecimalSeparator();
      decimal_separator_len = decimal_separator.Length();
      }
    if(((intptr)styleIn & (intptr)NumberStyles::AllowThousands) != 0)
      {
      group_separator = format->NumberGroupSeparator();
      group_separator_len = group_separator.Length();
      }
    if(((intptr)styleIn & (intptr)NumberStyles::AllowCurrencySymbol) != 0)
      {
      currency_symbol = format->CurrencySymbol();
      ps.currency_symbol_len = currency_symbol.Length();
      }
    String positive = format->PositiveSign();
    String negative = format->NegativeSign();
    ps.allow_trailing_parenthes = false;

    for(; ps.sidx < len; ps.sidx++)
      {
      ps.c = s [ps.sidx];

      if(ps.c == L'\0') 
        {
        ps.sidx = len;
        continue;
        }

      switch(ps.state)
        {
        case ParseState::AllowSign:
          {
          if(((intptr)styleIn & (intptr)NumberStyles::AllowLeadingSign) != 0)
            {
            if(ps.c == positive[0] && s.Substring(ps.sidx, positive.Length()) == positive)
              {
              ps.state = ParseState::Digits;
              ps.sidx += positive.Length() - 1;
              continue;
              }

            if(ps.c == negative[0] && s.Substring (ps.sidx, negative.Length()) == negative) 
              {
              ps.state = ParseState::Digits;
              b[ps.didx++] = (byte)'-';
              ps.sidx += negative.Length() - 1;
              continue;
              }
            }

          if(((intptr)styleIn & (intptr)NumberStyles::AllowParentheses) != 0 && ps.c == L'(') 
            {
            b[ps.didx++] = (byte)'-';
            ps.state = ParseState::Digits;
            ps.allow_trailing_parenthes = true;
            continue;
            }

          ps.state = ParseState::Digits;
          // Fall through to ParseState.Digits
          }
        case ParseState::Digits:
          {
          if(Char::IsDigit(ps.c)) 
            {
            b[ps.didx++] = (byte)ps.c;
            break;
            }

          if(ps.c == L'e' || ps.c == L'E')
            {
            int r = ParseDecimal(ps, exc);
            if(r == -1)
              break;
            else if(r == 0)
              return false;

            r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
            if(r == -1)
              continue;
            else if(r == 0)
              break;

            if(!ConsumeWhiteSpace(ps, exc))
              return false;
            break;
            }

          if(ps.allow_trailing_parenthes && ps.c == L')')
            {
            ps.allow_trailing_parenthes = false;
            ps.state = ParseState::ConsumeWhiteSpace;
            continue;
            }

          if(decimal_separator_len > 0 && decimal_separator[0] == ps.c) 
            {
            if(String::CompareOrdinal(s, ps.sidx, decimal_separator, 0, decimal_separator_len) == 0)
              {
              b[ps.didx++] = (byte)'.';
              ps.sidx += decimal_separator_len - 1;
              ps.state = ParseState::Decimal; 
              break;
              }
            }
          if(group_separator_len > 0 && group_separator [0] == ps.c)
            {
            if(s.Substring(ps.sidx, group_separator_len) == group_separator) 
              {
              ps.sidx += group_separator_len - 1;
              break;
              }
            }
          if(ps.currency_symbol_len > 0 && currency_symbol [0] == ps.c)
            {
            if(s.Substring (ps.sidx, ps.currency_symbol_len) == currency_symbol)
              {
              ps.sidx += ps.currency_symbol_len - 1;
              ps.currency_symbol_len = 0;
              break;
              }
            }

          ps.state = ParseState::TrailingSymbols;
          int r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
          if(r == -1)
            continue;
          else if(r == 0)
            break;
          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;
          }
        case ParseState::Decimal:
          {
          int r = ParseDecimal(ps, exc);
          if(r == -1)
            break;
          else if(r == 0)
            return false;

          r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
          if(r == -1)
            continue;
          else if(r == 0)
            break;
          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;
          }
        case ParseState::ExponentSign:
          {
          if(Char::IsDigit(ps.c))
            {
            ps.state = ParseState::Exponent;
            int r = ParseExponent(ps);
            if(r == 0)
              break;
            r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
            if(r == -1)
              continue;
            else if(r == 0)
              break;
            if(!ConsumeWhiteSpace(ps, exc))
              return false;
            break;
            }

          if(ps.c == positive[0] && s.Substring(ps.sidx, positive.Length()) == positive)
            {
            ps.state = ParseState::Digits;
            ps.sidx += positive.Length() - 1;
            continue;
            }

          if(ps.c == negative [0] && s.Substring(ps.sidx, negative.Length()) == negative)
            {
            ps.state = ParseState::Digits;
            b[ps.didx++] = (byte) '-';
            ps.sidx += negative.Length()-1;
            continue;
            }

          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;

          }
        case ParseState::Exponent:
          {
          int r = ParseExponent(ps);
          if(r == 0)
            break;
          r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
          if(r == -1)
            continue;
          else if(r == 0)
            break;
          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;
          }
        case ParseState::TrailingSymbols:
          {
          int r = ParseTrailingSymbols(ps, s, positive, negative, currency_symbol);
          if(r == -1)
            continue;
          else if(r == 0)
            break;
          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;
          }
        case ParseState::ConsumeWhiteSpace:
          {
          if(!ConsumeWhiteSpace(ps, exc))
            return false;
          break;
          }
        case ParseState::Exit:
        case ParseState::None:
        default:
          break;
        }

      if(ps.state == ParseState::Exit)
        break;
      }

    b[ps.didx] = 0;
    byte *p = &b[0];
    double retVal;
    if(!ParseImpl(p, retVal))
      {
      if(!tryParse)
        exc = Int32::GetFormatException ();
      return false;
      }
    if(IsPositiveInfinity(retVal) || IsNegativeInfinity(retVal))
      {
      if (!tryParse)
        //exc = OverflowException ();
          exc = SystemException(L"Overflow");
      return false;
      }

    result = retVal;
    return true;
    }

  bool Double::TryParseStringConstant(String format, String s, int start, int end)
    {
    return end - start + 1 == format.Length() && String::CompareOrdinal(format, 0, s, start, format.Length()) == 0;
    }

  bool Double::ParseImpl(byte* byte_ptr, double& value)
    {
    char *endptr = nullptr;
    value = 0.0;

    if(*byte_ptr)
      {
      //EnterCriticalSection (&mono_strtod_mutex);
      value = strtod((const char*)byte_ptr, &endptr);
      //LeaveCriticalSection (&mono_strtod_mutex);
      }

    if(!*byte_ptr || (endptr && *endptr))
      return false;

    return true;
    }

  int Double::ConsumeWhiteSpace(ParseStruct& ps, Exception& exc)
    {
    if(ps.allow_trailing_parenthes && ps.c == L')') 
      {
      ps.allow_trailing_parenthes = false;
      ps.state = ParseState::ConsumeWhiteSpace;
      return 0;
      }

    if(ps.allow_trailing_white && Char::IsWhiteSpace(ps.c)) 
      {
      ps.state = ParseState::ConsumeWhiteSpace;
      return 0;
      }

    if(!ps.tryParse)
      exc = FormatException(L"Unknown char");
    return 1;
    }

  int Double::ParseExponent(ParseStruct& ps)
    {
    if(Char::IsDigit(ps.c))
      {
      ps.b[ps.didx++] = (byte)ps.c;
      return 0;
      }

    ps.state = ParseState::TrailingSymbols;
    return 1;
    }

  int Double::ParseDecimal(ParseStruct& ps, Exception& exc)
    {
    using namespace Globalization;
    if(Char::IsDigit(ps.c))
      {
      ps.b[ps.didx++] = (byte)ps.c;
      return -1;
      }

    if(ps.c == L'e' || ps.c == L'E')
      {
      if( ((intptr)ps.style & (intptr)NumberStyles::AllowExponent) == (intptr)NumberStyles::None )
        {
        if(!ps.tryParse)
          exc = FormatException(L"Unknown char: " + ps.c);
        return 0;
        }
      ps.b[ps.didx++] = (byte)ps.c;
      ps.state = ParseState::ExponentSign;
      return -1;
      }

    ps.state = ParseState::TrailingSymbols;
    return 1;
    }

  int Double::ParseTrailingSymbols(ParseStruct& ps, String& s, String& positive, String& negative, String& currency_symbol)
    {
    using namespace Globalization;
    if(((intptr)ps.style & (intptr)NumberStyles::AllowTrailingSign) != 0)
      {
      if(ps.c == positive[0] && s.Substring(ps.sidx, positive.Length()) == positive) 
        {
        ps.state = ParseState::ConsumeWhiteSpace;
        ps.sidx += positive.Length() - 1;
        ps.allow_trailing_parenthes = false;
        positive = L"";
        return -1;
        }

      if(ps.c == negative[0] && s.Substring (ps.sidx, negative.Length()) == negative)
        {
        ps.state = ParseState::ConsumeWhiteSpace;
        ByteArray::Copy(ps.b, 0, ps.b, 1, ps.didx);
        ps.b[0] = (byte)'-';
        ++ps.didx;
        ps.sidx += negative.Length() - 1;
        ps.allow_trailing_parenthes = false;
        negative = L"";
        return -1;
        }
      }

    if(ps.currency_symbol_len > 0 && currency_symbol[0] == ps.c) 
      {
      if(s.Substring (ps.sidx, ps.currency_symbol_len) == currency_symbol)
        {
        ps.sidx += ps.currency_symbol_len - 1;
        ps.currency_symbol_len = 0;
        return 0;
        }
      }

    if(ps.allow_trailing_white && Char::IsWhiteSpace(ps.c))
      {
      return 0;
      }
    return 1;
    }
  }
