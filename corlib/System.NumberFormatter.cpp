// Perch Lake Computer System

#include "pch.h"
#include "System.NumberFormatter.h"
#include "NumberFormatEntry.h"
#include "AutoPtr.h"
#include "System.h"
//#include "System.Globalization.NumberFormatInfo.h"

namespace System
  {

  // ------------------------------------------------------------------------
  /// Default constructor
  NumberFormatter::NumberFormatter()
    :_ind(0)
    ,_cbuf()
    ,_positive(false)
    ,_specifier(L'\0')
    ,_precision(0)
    ,_defPrecision(0)
    ,_offset(0)
    ,_digitsLen(0)
    ,_NaN(false)
    ,_infinity(false)
    ,_isCustomFormat(false)
    ,_specifierIsUpper(false)
    ,_decPointPos(0)
    ,_val1(0)
    ,_val2(0)
    ,_val3(0)
    ,_val4(0)
    ,_nfi(nullptr)
    {
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Destructor
  NumberFormatter::~NumberFormatter()
    {
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Public Static NumberToString function (Based on Mono)
  String NumberFormatter::NumberToString(int32 value, IFormatProvider* fp)
    {
    if(value >= HundredMillion || value <= -HundredMillion)
      return NumberToString(nullptr, value, fp);

    GCNumberFormatter inst(GetInstance());
    String res = inst->FastIntegerToString(value, fp);
    return res;
    }
  // ------------------------------------------------------------------------

  String NumberFormatter::NumberToString(uint32 value, IFormatProvider* fp)
    {
    if(value >= HundredMillion)
      return NumberToString(nullptr, value, fp);

    // TODO : NumberFormatter inst = GetInstance (fp);
    GCNumberFormatter inst(GetInstance());
    String res = inst->FastIntegerToString((int)value, fp);
    return res;
    }

  String NumberFormatter::NumberToString(int64 value, IFormatProvider* fp)
    {
    if(value >= HundredMillion || value <= -HundredMillion)
      return NumberToString(nullptr, value, fp);

    // TODO : NumberFormatter inst = GetInstance (fp);
    GCNumberFormatter inst(GetInstance());
    String res = inst->FastIntegerToString((int32)value, fp);
    return res;
    }

  String NumberFormatter::NumberToString(float value, IFormatProvider* fp)
    {
    // TODO :GCNumberFormatter inst(GetInstance(fp));
    GCNumberFormatter inst(GetInstance());
    inst->Init(nullptr, value, SingleDefPrecision);
    Globalization::NumberFormatInfo* nfi = inst->GetNumberFormatInstance(fp);
    String res;
    if(inst->_NaN)
      res = nfi->NaNSymbol();
    else if(inst->_infinity)
      if(inst->_positive)
        res = nfi->PositiveInfinitySymbol();
      else
        res = nfi->NegativeInfinitySymbol();
    else
      res = inst->FormatGeneral(-1, nfi);
    return res;
    }

  // ------------------------------------------------------------------------
  /// Public Static NumberToString function (Based on Mono)
  String NumberFormatter::NumberToString(String* format, int32 value, IFormatProvider* fp)
    {
    GCNumberFormatter inst(GetInstance());
    inst->Init (format, value, UInt32DefPrecision);
    String res = inst->IntegerToString(format, fp);
    return res;
    }
  // ------------------------------------------------------------------------

  String NumberFormatter::NumberToString(String* format, uint32 value, IFormatProvider* fp)
    {
    // TODO : NumberFormatter inst = GetInstance (fp);
    GCNumberFormatter inst(GetInstance());
    inst->Init(format, value, Int32DefPrecision);
    String res = inst->IntegerToString(format, fp);
    return res;
    }

  String NumberFormatter::NumberToString(String* format, int64 value, IFormatProvider* fp)
    {
    // TODO : NumberFormatter inst = GetInstance (fp);
    GCNumberFormatter inst(GetInstance());
    inst->Init(format, value);
    String res = inst->IntegerToString(format, fp);
    return res;
    }

  String NumberFormatter::NumberToString(String* format, float value, IFormatProvider* fp)
    {
    // TODO :GCNumberFormatter inst(GetInstance(fp));
    GCNumberFormatter inst(GetInstance());
    inst->Init(format, value, SingleDefPrecision);
    Globalization::NumberFormatInfo* nfi = inst->GetNumberFormatInstance(fp);
    //NumberFormatInfo nfi = inst.GetNumberFormatInstance (fp);
    String res;
    if(inst->_NaN)
      res = nfi->NaNSymbol();
    else if(inst->_infinity)
      if (inst->_positive)
        res = nfi->PositiveInfinitySymbol();
      else
        res = nfi->NegativeInfinitySymbol();
    // TODO else if (inst->_specifier == 'R')
    // res = inst->FormatRoundtrip(value, nfi);
    else
      res = inst->NumberToString(format, nfi);
    return res;
    }

  // ------------------------------------------------------------------------
  /// Private Static NumberToString function (Based on Mono)
  NumberFormatter* NumberFormatter::GetInstance()
    {
    return new NumberFormatter();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private Init function (Based on Mono)
  void NumberFormatter::Init(String* format, int value, int defPrecision)
    {
    Init(format);
    _defPrecision = defPrecision;
    _positive = value >= 0;

    if(value == 0 || _specifier == L'X')
      {
      InitHex(value);
      return;
      }

    InitDecHexDigits((uint32)abs(value));
    _decPointPos = _digitsLen = DecHexLen();
    }
  // ------------------------------------------------------------------------

  void NumberFormatter::Init(String* format, uint32 value, int defPrecision)
    {
    Init(format);
    _defPrecision = defPrecision;
    _positive = true;

    if(value == 0 || _specifier == L'X') 
      {
      InitHex(value);
      return;
      }

    InitDecHexDigits(value);
    _decPointPos = _digitsLen = DecHexLen ();
    }

  void NumberFormatter::Init(String* format, int64 value)
    {
    Init(format);
    _defPrecision = Int64DefPrecision;
    _positive = value >= 0;

    if(value == 0 || _specifier == L'X')
      {
      InitHex((uint64)value);
      return;
      }

    if(value < 0)
      value = -value;
    InitDecHexDigits((uint64)value);
    _decPointPos = _digitsLen = DecHexLen ();
    }

  void NumberFormatter::Init(String* format, double value, int defPrecision)
    {
    Init(format);

    _defPrecision = defPrecision;
    int64 bits = BitConverter::DoubleToInt64Bits(value);
    _positive = bits >= 0;
    bits &= Int64::MaxValue;
    if(bits == 0)
      {
      _decPointPos = 1;
      _digitsLen = 0;
      _positive = true;
      return;
      }

    int e = (int)((int64)bits >> DoubleBitsExponentShift);
    int64 m = bits & DoubleBitsMantissaMask;
    if(e == DoubleBitsExponentMask) 
      {
      _NaN = m != 0;
      _infinity = m == 0;
      return;
      }

    int expAdjust = 0;
    if(e == 0)
      {
      // We need 'm' to be large enough so we won't lose precision.
      e = 1;
      int32 scale = ScaleOrder(m);
      if(scale < DoubleDefPrecision)
        {
        expAdjust = scale - DoubleDefPrecision;
        m *= (long)GetTenPowerOf (-expAdjust);
        }
      }
    else 
      {
      m = (m + DoubleBitsMantissaMask + 1) * 10;
      expAdjust = -1;
      }

    // multiply the mantissa by 10 ^ N
    uint64 lo = (uint32)m;
    uint64 hi = m >> 32;
    uint64 lo2 = MantissaBitsTable[e];
    uint64 hi2 = lo2 >> 32;
    lo2 = (uint32)lo2;
    uint64 mm = hi * lo2 + lo * hi2 + ((lo * lo2) >> 32);
    int64 res = (int64)(hi * hi2 + (mm >> 32));
    while(res < SeventeenDigitsThreshold)
      {
      mm = (mm & UInt32::MaxValue) * 10;
      res = res * 10 + (int64)(mm >> 32);
      expAdjust--;
      }
    if((mm & 0x80000000) != 0)
      res++;

    int order = DoubleDefPrecision + 2;
    _decPointPos = TensExponentTable[e] + expAdjust + order;

    // Rescale 'res' to the initial precision (15-17 for doubles).
    int initialPrecision = InitialFloatingPrecision();
    if(order > initialPrecision) 
      {
      int64 val = GetTenPowerOf (order - initialPrecision);
      res = (res + (val >> 1)) / val;
      order = initialPrecision;
      }
    if(res >= GetTenPowerOf(order))
      {
      order++;
      _decPointPos++;
      }

    InitDecHexDigits((uint64)res);
    _offset = CountTrailingZeros();
    _digitsLen = order - _offset;
    }

#pragma warning(disable:4715)
  String NumberFormatter::IntegerToString(String*, IFormatProvider* fp)
    {
    Globalization::NumberFormatInfo* nfi = GetNumberFormatInstance(fp);
    switch(_specifier) 
      {
      case 'C':
        return FormatCurrency(_precision, nfi);
      case 'D':
        return FormatDecimal(_precision, nfi);
        //case 'E':
        //return FormatExponential (_precision, nfi);
        //case 'F':
        //return FormatFixedPoint (_precision, nfi);
      case 'G':
        if (_precision <= 0)
          return FormatDecimal(-1, nfi);
        return FormatGeneral(_precision, nfi);
        //case 'N':
        //return FormatNumber (_precision, nfi);
        //case 'P':
        //return FormatPercent (_precision, nfi);
      case 'X':
        return FormatHexadecimal(_precision);
        //default:
        //if (_isCustomFormat)
        //return FormatCustom (format, nfi);
        //throw FormatException ("The specified format '" + format + "' is invalid");
      }
    }

  String NumberFormatter::NumberToString(String*, Globalization::NumberFormatInfo* nfi)
    {
    wchar_t specifier = _specifier;
    switch (specifier)
      {
      case 'C':
        return FormatCurrency(_precision, nfi);
      case 'D':
        //IL_24:
        switch (specifier)
          {
          // TODO : case 'N':
          // return FormatNumber(_precision, nfi);
          case 'O':
            //IL_39:
            if (specifier != 'X')
              {
              }
            // TODO : if (this._isCustomFormat)
            //{
            //return this.FormatCustom(format, nfi);
            //}
            throw SystemException(L"The specified format is invalid");
            // TODO : throw new FormatException("The specified format '" + format + "' is invalid");
            // TODO : case 'P':
            //return this.FormatPercent(this._precision, nfi);
          }
        //goto IL_39;
        // TODO : case 'E':
        //return this.FormatExponential(this._precision, nfi);
        // TODO : case 'F':
        //return this.FormatFixedPoint(this._precision, nfi);
      case 'G':
        return FormatGeneral(_precision, nfi);
      }
    //goto IL_24;
    }

  // ------------------------------------------------------------------------
  /// Private GetNumberFormatInstance function (Based on Mono)
  Globalization::NumberFormatInfo* NumberFormatter::GetNumberFormatInstance(IFormatProvider* fp)
    {
    if (_nfi != nullptr && fp == nullptr)
      return _nfi;
    return Globalization::NumberFormatInfo::GetInstance(fp);
    }
  // ------------------------------------------------------------------------

  void NumberFormatter::AddOneToDecHex ()
    {
    if (_val1 == 0x99999999)
      {
      _val1 = 0;
      if (_val2 == 0x99999999)
        {
        _val2 = 0;
        if (_val3 == 0x99999999) 
          {
          _val3 = 0;
          _val4 = AddOneToDecHex(_val4);
          }
        else
          _val3 = AddOneToDecHex(_val3);
        }
      else
        _val2 = AddOneToDecHex(_val2);
      }
    else
      _val1 = AddOneToDecHex(_val1);
    }

  // static
  uint32 NumberFormatter::AddOneToDecHex(uint32 val)
    {
    if ((val & 0xffff) == 0x9999)
      if ((val & 0xffffff) == 0x999999)
        if ((val & 0xfffffff) == 0x9999999)
          return val + 0x06666667;
        else
          return val + 0x00666667;
      else if ((val & 0xfffff) == 0x99999)
        return val + 0x00066667;
      else
        return val + 0x00006667;
    else if ((val & 0xff) == 0x99)
      if ((val & 0xfff) == 0x999)
        return val + 0x00000667;
      else
        return val + 0x00000067;
    else if ((val & 0xf) == 0x9)
      return val + 0x00000007;
    else
      return val + 1;
    }

  // ------------------------------------------------------------------------
  /// Private Append function (Based on Mono)
  void NumberFormatter::Append(String& s)
    {
    int slen = s.Length();
    if (_ind + slen > _cbuf.Length())
      Resize(_ind + slen + 10);
    for (int i = 0; i < slen; i++)
      _cbuf[_ind++] = s[i];
    }
  // ------------------------------------------------------------------------

  void NumberFormatter::Append(cstring s)
    {
    String temp(s);
    Append(temp);
    }

  void NumberFormatter::Append(wchar_t c)
    {
    String temp(c, 1);
    Append(temp);
    }

  // ------------------------------------------------------------------------
  /// Private AppendDigits function (Based on Mono)
  void NumberFormatter::AppendDigits(int start, int end)
    {
    if (start >= end)
      return;

    int i = _ind + (end - start);
    if (i >= (int)_cbuf.Length())
      Resize (i + 10);
    _ind = i;

    _cbuf[i] = L'\0';

    end += _offset;
    start += _offset;

    for (int next = start + 8 - (start & 0x7); ; start = next, next += 8)
      {
      uint32 v;
      if (next == 8)
        v = _val1;
      else if (next == 16)
        v = _val2;
      else if (next == 24)
        v = _val3;
      else if (next == 32)
        v = _val4;
      else
        v = 0;
      v >>= (start & 0x7) << 2;
      if (next > end)
        next = end;

      _cbuf [--i] = (wchar_t)(L'0' | v & 0xf);
      switch (next - start)
        {
        case 8:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 7:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 6:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 5:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 4:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 3:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 2:
          _cbuf [--i] = (wchar_t)(L'0' | (v >>= 4) & 0xf);
        case 1:
          if (next == end)
            return;
          continue;
        }
      }
    }
  // ------------------------------------------------------------------------

  void NumberFormatter::AppendDecimalString(int precision)
    {
    AppendDigits(_digitsLen - precision - _decPointPos, _digitsLen - _decPointPos);
    }

  int NumberFormatter::CountTrailingZeros()
    {
    if (_val1 != 0)
      return CountTrailingZeros (_val1);
    if (_val2 != 0)
      return CountTrailingZeros (_val2) + 8;
    if (_val3 != 0)
      return CountTrailingZeros (_val3) + 16;
    if (_val4 != 0)
      return CountTrailingZeros (_val4) + 24;
    return _digitsLen;
    }

  void NumberFormatter::AppendIntegerStringWithGroupSeparator(IntArray& groups, String groupSeparator)
    {
    if(IsZeroInteger())
      {
      Append ('0');
      return;
      }

    int total = 0;
    int groupIndex = 0;
    for(int i = 0; i < (int)groups.Length(); i++) 
      {
      total += groups[i];
      if (total <= _decPointPos)
        groupIndex = i;
      else
        break;
      }

    if(groups.Length() > 0 && total > 0) 
      {
      int counter;
      int groupSize = groups[groupIndex];
      int fraction = _decPointPos > total ? _decPointPos - total : 0;
      if (groupSize == 0) 
        {
        while(groupIndex >= 0 && groups[groupIndex] == 0)
          groupIndex--;

        groupSize = fraction > 0 ? fraction : groups[groupIndex];
        }
      if(fraction == 0)
        counter = groupSize;
      else 
        {
        groupIndex += fraction / groupSize;
        counter = fraction % groupSize;
        if(counter == 0)
          counter = groupSize;
        else
          groupIndex++;
        }

      if(total >= _decPointPos) 
        {
        int lastGroupSize = groups[0];
        if(total > lastGroupSize)
          {
          int lastGroupDiff = -(lastGroupSize - _decPointPos);
          int lastGroupMod;

          if(lastGroupDiff < lastGroupSize)
            counter = lastGroupDiff;
          else if(lastGroupSize > 0 && (lastGroupMod = _decPointPos % lastGroupSize) > 0)
            counter = lastGroupMod;
          }
        }

      for(int i = 0; ;) 
        {
        if((_decPointPos - i) <= counter || counter == 0) 
          {
          AppendDigits (_digitsLen - _decPointPos, _digitsLen - i);
          break;
          }
        AppendDigits (_digitsLen - i - counter, _digitsLen - i);
        i += counter;
        Append(groupSeparator);
        if (--groupIndex < (int)groups.Length() && groupIndex >= 0)
          groupSize = groups [groupIndex];
        counter = groupSize;
        }
      }
    else 
      {
      AppendDigits(_digitsLen - _decPointPos, _digitsLen);
      }
    }

  int NumberFormatter::CountTrailingZeros(uint32 val)
    {
    if ((val & 0xffff) == 0)
      if ((val & 0xffffff) == 0)
        if ((val & 0xfffffff) == 0)
          return 7;
        else
          return 6;
      else if ((val & 0xfffff) == 0)
        return 5;
      else
        return 4;
    else if ((val & 0xff) == 0)
      if ((val & 0xfff) == 0)
        return 3;
      else
        return 2;
    else if ((val & 0xf) == 0)
      return 1;
    else
      return 0;
    }

  // ------------------------------------------------------------------------
  /// Private DecHexLen function (Based on Mono)
  int NumberFormatter::DecHexLen()
    {
    if (_val4 != 0)
      return DecHexLen(_val4) + 24;
    else if (_val3 != 0)
      return DecHexLen(_val3) + 16;
    else if (_val2 != 0)
      return DecHexLen(_val2) + 8;
    else if (_val1 != 0)
      return DecHexLen(_val1);
    else
      return 0;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private DecHexLen function (Based on Mono)
  int NumberFormatter::DecHexLen(uint32 val)
    {
    if (val < 0x10000)
      return FastDecHexLen ((int)val);
    return 4 + FastDecHexLen ((int)(val >> 16));
    }
  // ------------------------------------------------------------------------

  int64 NumberFormatter::GetTenPowerOf(int i)
    {
    return TenPowersList[i];
    }

  // ------------------------------------------------------------------------
  /// Private FastAppendDigits function (Based on Mono)
  void NumberFormatter::FastAppendDigits(int val, bool force)
    {
    int i = _ind;
    int digits = 0;
    if (force || val >= 100) 
      {
      int v = (val * 5243) >> 19;
      digits = DecHexDigits[v];
      if (force || val >= 1000)
        _cbuf [i++] = (wchar_t)(L'0' | digits >> 4);
      _cbuf [i++] = (wchar_t)(L'0' | (digits & 0xf));
      digits = DecHexDigits[val - v * 100];
      }
    else
      digits = DecHexDigits[val];

    if (force || val >= 10)
      _cbuf [i++] = (wchar_t)(L'0' | digits >> 4);
    _cbuf [i++] = (wchar_t)(L'0' | (digits & 0xf));
    _ind = i;
    _cbuf[_ind] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private FastAppendDigits function (Based on Mono)
  uint32 NumberFormatter::FastToDecHex(int val)
    {
    if (val < 100)
      return DecHexDigits[val];

    // Uses 2^19 (524288) to compute val / 100 for val < 10000.
    int v = (val * 5243) >> 19;
    return ((DecHexDigits [v] << 8) | DecHexDigits [val - v * 100]);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private FastAppendDigits function (Based on Mono)
  int NumberFormatter::FastDecHexLen(int val)
    {
    if (val < 0x100)
      if (val < 0x10)
        return 1;
      else
        return 2;
    else if (val < 0x1000)
      return 3;
    else
      return 4;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private FastIntegerToString function (Based on Mono)
  String NumberFormatter::FastIntegerToString(int value, IFormatProvider* fp)
    {
    if(value < 0)
      {
      String sign = GetNumberFormatInstance(fp)->NegativeSign();
      ResetCharBuf (8 + sign.Length());
      value = -value;
      Append(sign);
      }
    else
      ResetCharBuf(8);

    if (value >= 10000)
      {
      int v = value / 10000;
      FastAppendDigits (v, false);
      FastAppendDigits (value - v * 10000, true);
      }
    else
      FastAppendDigits(value, false);

    return String(_cbuf.ToConstPtr(), 0, _ind);
    }
  // ------------------------------------------------------------------------

  String NumberFormatter::FormatCurrency(int precision, Globalization::NumberFormatInfo* nfi)
    {
    precision = (precision >= 0 ? precision : nfi->CurrencyDecimalDigits());
    RoundDecimal(precision);
    ResetCharBuf(IntegerDigits() * 2 + precision * 2 + 16);

    if(_positive) 
      {
      switch(nfi->CurrencyPositivePattern()) 
        {
        case 0:
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 2:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L' ');
          break;
        }
      }
    else
      {
      switch(nfi->CurrencyNegativePattern()) 
        {
        case 0:
          Append(L'(');
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 1:
          Append((string)(cstring)nfi->NegativeSign());
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 2:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 3:
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 4:
          Append(L'(');
          break;
        case 5:
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 8:
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 9:
          Append((string)(cstring)nfi->NegativeSign());
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L' ');
          break;
        case 11:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L' ');
          break;
        case 12:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L' ');
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 14:
          Append(L'(');
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L' ');
          break;
        case 15:
          Append(L'(');
          break;
        }
      }

    AppendIntegerStringWithGroupSeparator(nfi->RawCurrencyGroupSizes(), nfi->CurrencyGroupSeparator());

    if(precision > 0) 
      {
      Append((string)(cstring)nfi->CurrencyDecimalSeparator());
      AppendDecimalString(precision);
      }

    if(_positive)
      {
      switch(nfi->CurrencyPositivePattern()) 
        {
        case 1:
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 3:
          Append(L' ');
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        }
      }
    else
      {
      switch(nfi->CurrencyNegativePattern()) 
        {
        case 0:
          Append(L')');
          break;
        case 3:
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 4:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L')');
          break;
        case 5:
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 6:
          Append((string)(cstring)nfi->NegativeSign());
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 7:
          Append((string)(cstring)nfi->CurrencySymbol());
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 8:
          Append(L' ');
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 10:
          Append(L' ');
          Append((string)(cstring)nfi->CurrencySymbol());
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 11:
          Append((string)(cstring)nfi->NegativeSign());
          break;
        case 13:
          Append((string)(cstring)nfi->NegativeSign());
          Append(L' ');
          Append((string)(cstring)nfi->CurrencySymbol());
          break;
        case 14:
          Append(L')');
          break;
        case 15:
          Append(L' ');
          Append((string)(cstring)nfi->CurrencySymbol());
          Append(L')');
          break;
        }
      }

    return String(_cbuf.ToConstPtr(), 0, _ind);
    }

  // ------------------------------------------------------------------------
  /// Private FormatDecimal function (Based on Mono)
  String NumberFormatter::FormatDecimal(int precision, Globalization::NumberFormatInfo* nfi)
    {
    if (precision < _digitsLen)
      precision = _digitsLen;
    if (precision == 0)
      return String(L"0");

    ResetCharBuf (precision + 1);
    if (!_positive)
      {
      String neg = nfi->NegativeSign();
      Append(neg);
      }
    AppendDigits (0, precision);

    return String(_cbuf.ToConstPtr(), 0, _ind);
    }
  // ------------------------------------------------------------------------

  String NumberFormatter::FormatExponential(int precision, Globalization::NumberFormatInfo* nfi)
    {
    if (precision == -1)
      precision = DefaultExpPrecision;

    RoundPos (precision + 1);
    return FormatExponential (precision, nfi, 3);
    }

#pragma warning (disable:4189)
  String NumberFormatter::FormatExponential(int precision, Globalization::NumberFormatInfo* nfi, int)
    {
    int decDigits = _decPointPos;
    int digits = _digitsLen;
    int exponent = decDigits - 1;
    decDigits = _decPointPos = 1;

    ResetCharBuf(precision + 8);

    if(!_positive)
      Append((string)(cstring)nfi->NegativeSign());

    //AppendOneDigit(digits - 1);

    if(precision > 0)
      {
      //Append(nfi.NumberDecimalSeparator);
      AppendDigits (digits - precision - 1, digits - _decPointPos);
      }

    //AppendExponent(nfi, exponent, expDigits);

    return String(_cbuf.ToConstPtr(), 0, _ind);
    }
#pragma warning(default:4189)

  String NumberFormatter::FormatGeneral(int precision, Globalization::NumberFormatInfo* nfi)
    {
    bool enableExp;
    if (precision == -1)
      {
      enableExp = IsFloatingSource();
      precision = _defPrecision;
      }
    else
      {
      enableExp = true;
      if (precision == 0)
        precision = _defPrecision;
      RoundPos (precision);
      }

    int intDigits = _decPointPos;
    int digits = _digitsLen;
    int decDigits = digits - intDigits;

    if((intDigits > precision || intDigits <= -4) && enableExp)
      return FormatExponential(digits - 1, nfi, 2);

    if (decDigits < 0)
      decDigits = 0;
    if (intDigits < 0)
      intDigits = 0;
    ResetCharBuf(decDigits + intDigits + 3);

    if(!_positive)
      {
      String neg = nfi->NegativeSign();
      Append(neg);
      }

    if(intDigits == 0)
      Append(L'0');
    else
      AppendDigits(digits - intDigits, digits);

    if(decDigits > 0)
      {
      Append((cstring)nfi->NumberDecimalSeparator());
      AppendDigits(0, decDigits);
      }

    return String(_cbuf.ToConstPtr(), 0, _ind);
    }

  // ------------------------------------------------------------------------
  /// Private FormatHexadecimal function (Based on Mono)
  String NumberFormatter::FormatHexadecimal(int precision)
    {
    int size = Math::Max(precision, _decPointPos);
    wchar_t* digits = _specifierIsUpper ? (wchar_t*)DigitUpperTable : (wchar_t*)DigitLowerTable;
    ResetCharBuf(size + 10);
    _ind = size;
    _cbuf[size] = L'\0';
    ulong val = _val1 | ((uint64)_val2 << 32);
    while (size > 0)
      {
      _cbuf [--size] = digits [val & 0xf];
      val >>= 4;
      }
    CharArray chars(1);
    chars[0] = L'0';
    return String(_cbuf.ToConstPtr(), 0, _ind).TrimStart(chars);
    }
  // ------------------------------------------------------------------------

  int NumberFormatter::InitialFloatingPrecision()
    {
    if (_specifier == 'R')
      return _defPrecision + 2;
    if (_precision < _defPrecision)
      return _defPrecision;
    if (_specifier == 'G')
      return Math::Min(_defPrecision + 2, _precision);
    if (_specifier == 'E')
      return Math::Min(_defPrecision + 2, _precision + 1);
    return _defPrecision;
    }

  void NumberFormatter::Init(String* format)
    {
    _val1 = _val2 = _val3 = _val4 = 0;
    _offset = 0;
    _NaN = _infinity = false;
    _isCustomFormat = false;
    _specifierIsUpper = true;
    _precision = -1;

    if (format == nullptr || format->Length() == 0)
      {
      _specifier = 'G';
      return;
      }

    wchar_t specifier = *format[0];
    if (specifier >= L'a' && specifier <= L'z') 
      {
      specifier = (wchar_t)(specifier - L'a' + L'A');
      _specifierIsUpper = false;
      }
    else if (specifier < L'A' || specifier > L'Z') 
      {
      _isCustomFormat = true;
      _specifier = L'0';
      return;
      }
    _specifier = specifier;
    if (format->Length() > 1) 
      {
      // TODO: _precision = ParsePrecision (format);
      if (_precision == -2)
        { // Is it a custom format?
        _isCustomFormat = true;
        _specifier = '0';
        _precision = -1;
        }
      }
    }
#pragma warning (default:4715)

  // ------------------------------------------------------------------------
  /// Private InitDecHexDigitsfunction (Based on Mono)
  void NumberFormatter::InitDecHexDigits(uint32 value)
    {
    if (value >= HundredMillion) 
      {
      uint32 div1 = (uint32)(value / HundredMillion);
      value -= HundredMillion * div1;
      _val2 = FastToDecHex(div1);
      }
    _val1 = ToDecHex(value);
    }
  // ------------------------------------------------------------------------

  // Translate an unsigned long to hexadecimal digits.
  void NumberFormatter::InitDecHexDigits(uint64 value)
    {
    if (value >= HundredMillion) {
      long div1 = (long)(value / HundredMillion);
      value -= HundredMillion * (ulong)div1;
      if (div1 >= HundredMillion) {
        int div2 = (int)(div1 / HundredMillion);
        div1 = div1 - div2 * (long)HundredMillion;
        _val3 = ToDecHex (div2);
        }
      if (div1 != 0)
        _val2 = ToDecHex ((int)(div1));
      }
    if (value != 0)
      _val1 = ToDecHex ((int)value);
    }

  // ------------------------------------------------------------------------
  /// Private InitHex function (Based on Mono)
  void NumberFormatter::InitHex(uint64 value)
    {
    switch (_defPrecision)
      {
      case Int8DefPrecision:  value = (byte) value;    break;
      case Int16DefPrecision: value = (uint16) value;  break;
      case Int32DefPrecision: value = (uint32) value;    break;
      }
    _val1 = (uint32)value;
    _val2 = (uint32)(value >> 32);
    _decPointPos = _digitsLen = DecHexLen ();
    if (value == 0)
      _decPointPos = 1;
    }
  // ------------------------------------------------------------------------

  bool NumberFormatter::IsFloatingSource()
    {
    return _defPrecision == DoubleDefPrecision || _defPrecision == SingleDefPrecision;
    }

  bool NumberFormatter::IsZeroInteger()
    {
    return _digitsLen == 0 || _decPointPos <= 0;
    }

  void NumberFormatter::RemoveTrailingZeros()
    {
    _offset = CountTrailingZeros ();
    _digitsLen -= _offset;
    if (_digitsLen == 0)
      {
      _offset = 0;
      _decPointPos = 1;
      _positive = true;
      }
    }

  int NumberFormatter::IntegerDigits()
    {
    return _decPointPos > 0 ? _decPointPos : 1;
    } 

  // ------------------------------------------------------------------------
  /// Private ResetCharBuf function (Based on Mono)
  void NumberFormatter::ResetCharBuf(uint32 size)
    {
    _ind = 0;
    if(_cbuf.Length() < size)
      _cbuf.Length(size);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Private Resize function (Based on Mono)
  void NumberFormatter::Resize(uint32 len)
    {
    _cbuf.Length(len);
    }
  // ------------------------------------------------------------------------

  bool NumberFormatter::RoundBits(int shift)
    {
    if (shift <= 0)
      return false;

    if (shift > _digitsLen)
      {
      _digitsLen = 0;
      _decPointPos = 1;
      _val1 = _val2 = _val3 = _val4 = 0;
      _positive = true;
      return false;
      }
    shift += _offset;
    _digitsLen += _offset;
    while (shift > 8)
      {
      _val1 = _val2;
      _val2 = _val3;
      _val3 = _val4;
      _val4 = 0;
      _digitsLen -= 8;
      shift -= 8;
      }
    shift = (shift - 1) << 2;
    uint32 v = _val1 >> shift;
    uint32 rem16 = v & 0xf;
    _val1 = (v ^ rem16) << shift;
    bool res = false;
    if (rem16 >= 0x5)
      {
      _val1 |= 0x99999999 >> (28 - shift);
      AddOneToDecHex();
      int newlen = DecHexLen ();
      res = newlen != _digitsLen;
      _decPointPos = _decPointPos + newlen - _digitsLen;
      _digitsLen = newlen;
      }
    RemoveTrailingZeros ();
    return res;
    }

  bool NumberFormatter::RoundDecimal(int decimals)
    {
    return RoundBits(_digitsLen - _decPointPos - decimals);
    }

  void NumberFormatter::RoundPos(int pos)
    {
    RoundBits(_digitsLen - pos);
    }

  int NumberFormatter::ScaleOrder(int64 hi)
    {
    for(int i = TenPowersListLength - 1; i >= 0; i--)
      if (hi >= GetTenPowerOf(i))
        return i + 1;
    return 1;
    }

  // ------------------------------------------------------------------------
  /// Private ToDecHex function (Based on Mono)
  uint32 NumberFormatter::ToDecHex(int val)
    {
    uint32 res = 0;
    if (val >= 10000)
      {
      int v = val / 10000;
      val -= v * 10000;
      res = FastToDecHex (v) << 16;
      }
    return res | FastToDecHex (val);
    }
  // ------------------------------------------------------------------------

  } // namespace System

