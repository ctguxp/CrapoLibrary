// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"
#include "System.IFormatProvider.h"

namespace System
  {
  class Exception;
  class CRAPOCOREDLL_API Int32 : public Object
    {
    public:
      static const int32 MaxValue = 2147483647;
      static const int32 MinValue = (-2147483647 - 1);
    private:
      int32 _value;
    public:
      Int32();
      Int32(int32 const&);
      ~Int32();
      Int32& operator = (int32 const&);
      operator int32 const& () const;
      virtual String ToString() override;
      String ToString(String format);
      String ToString(String format, IFormatProvider* provider);
      virtual uint32 GetHashCode() override;
      static Exception GetFormatException();
      static int Parse(String s); 
    private:
      static bool Parse(String s, bool tryParse, int& result, Exception& exc);
      static bool ProcessTrailingWhitespace(bool tryParse, String s, int position, Exception& exc);
    };
  }

