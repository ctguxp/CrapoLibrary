#pragma once
#include "System.Object.h"
#include "System.IFormatProvider.h"

namespace System
  {
  class CRAPOCOREDLL_API Int64 : public Object
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
      virtual String ToString() override;
      String ToString(IFormatProvider* /*provider*/);
      virtual uint32 GetHashCode() override;
    };
  }
