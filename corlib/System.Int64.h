#pragma once
#include "System.Object.h"
#include "System.IComparable.h"
#include "System.IFormatProvider.h"
#include "System.Globalization.NumberStyles.h"
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API Int64 : public Object, public IComparable
    {
    public:
    enum : int64
      {
      MaxValue = 9223372036854775807i64,
      MinValue = (-9223372036854775807i64 - 1)
      };
    private:
      int64 _value;
    public:
      Int64();
      Int64(int64 const&);
      ~Int64();
      Int64& operator = (int64 const&);
      operator int64 const& () const;
      virtual bool Int64::Equals(Object* /*obj*/) override;
      virtual int CompareTo(Object& /*value*/) override;
      virtual String ToString() override;
      String ToString(IFormatProvider* /*provider*/);
      virtual uint32 GetHashCode() override;
      static bool TryParse(String s, Globalization::NumberStyles style, IFormatProvider* provider, int64& result);
    protected:
      static bool Parse(String s, Globalization::NumberStyles style, IFormatProvider* fp, bool tryParse, int64& result, GCException& exc);
    };
  }
