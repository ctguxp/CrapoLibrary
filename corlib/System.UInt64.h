#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API UInt64 : public Object
    {
    public:
      enum : uint64
        {
        MaxValue = 0xffffffffffffffffui64,
        MinValue = 0
        };
    private:
      uint64 _value;
    public:
      UInt64();
      UInt64(uint64 const&);
      ~UInt64();
      UInt64& operator = (uint64 const&);
      operator uint64 const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }
