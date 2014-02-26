#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API UInt32 : public Object
    {
    public:
      enum : uint32
        {
        MaxValue = 0xffffffffui32,
        MinValue = 0
        };
    private:
      uint32 _value;
    public:
      UInt32();
      UInt32(uint32 const&);
      ~UInt32();
      UInt32& operator = (uint32 const&);
      operator uint32 const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }
