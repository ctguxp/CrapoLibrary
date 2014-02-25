// Perch Lake Computer System

#pragma once
#include "AutoPtr.h"
#include "System.Array.h"
#include "System.String.h"
#include "System.IFormatProvider.h"
#include "System.Globalization.NumberFormatInfo.h"

namespace System
  {
  static const uint32 DecHexDigits[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
                                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 
                                       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 
                                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 
                                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
                                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 
                                       0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
                                       0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99 };

  static const wchar_t DigitLowerTable [] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'a', L'b', L'c', L'd', L'e', L'f' };

  static const wchar_t DigitUpperTable [] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };

  class NumberFormatter
    {
    static const int DefaultExpPrecision = 6;
    static const int HundredMillion = 100000000;
    static const int SingleDefPrecision = 7;
    static const int DoubleDefPrecision = 15;
    static const int Int8DefPrecision = 3;
    static const int UInt32DefPrecision = 10;
    static const int Int16DefPrecision = 5;
    static const int Int32DefPrecision = 10;

    public:
      ~NumberFormatter();
      static String NumberToString(int, IFormatProvider*);
      static String NumberToString(String*, int, IFormatProvider*);
      String FormatExponential(int precision, Globalization::NumberFormatInfo* nfi);
      String FormatCurrency(int precision, Globalization::NumberFormatInfo* nfi);
    private:
      NumberFormatter();
      void Append(cstring s);
      void Append(String&);
      void Append(wchar_t c);
      void AppendDigits(int start, int end);
      void AppendDecimalString(int precision);
      void AddOneToDecHex();
      static uint32 AddOneToDecHex(uint32 val);
      void AppendIntegerStringWithGroupSeparator(IntArray& groups, String groupSeparator);
      int CountTrailingZeros();
      static int CountTrailingZeros(uint32 val);
      String FormatExponential(int precision, Globalization::NumberFormatInfo* nfi, int expDigits);
      static NumberFormatter* GetInstance();
      static uint32 FastToDecHex(int val);
      Globalization::NumberFormatInfo* GetNumberFormatInstance(IFormatProvider*);
      String FastIntegerToString (int, IFormatProvider*);
      String FormatGeneral(int precision, Globalization::NumberFormatInfo* nfi);
      void Init(String*);
      void Init(String*, int, int);
      void InitDecHexDigits(uint32);
      void InitHex(ulong);
      int IntegerDigits();
      String IntegerToString(String*, IFormatProvider*);
      bool IsFloatingSource();
      bool IsZeroInteger();
      void FastAppendDigits(int, bool);
      String FormatDecimal(int precision, Globalization::NumberFormatInfo* nfi);
      String FormatHexadecimal(int precision);
      int DecHexLen();
      static int DecHexLen (uint32 val);
      static int FastDecHexLen(int val);
      void RemoveTrailingZeros();
      void ResetCharBuf(uint32);
      void Resize(uint32);
      bool RoundBits(int shift);
      bool RoundDecimal(int decimals);
      void RoundPos(int pos);
      static uint32 ToDecHex(int val);
    private:
      uint32       _ind;
      CharArray _cbuf;
      bool      _positive;
      wchar_t   _specifier;
      int _precision;
      int       _defPrecision;
      int _offset;
      int _digitsLen;
      bool _NaN;
      bool _infinity;
      bool _isCustomFormat;
      bool _specifierIsUpper;
      int _decPointPos;
      uint32 _val1;
      uint32 _val2;
      uint32 _val3;
      uint32 _val4;
      Globalization::NumberFormatInfo* _nfi;
    };

  typedef AutoPtr<NumberFormatter> GCNumberFormatter;
  }
