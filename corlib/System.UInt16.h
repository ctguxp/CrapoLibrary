#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API UInt16 : public Object
    {
    public:
      enum : uint16
        {
        MaxValue = 0xffffui16,
        MinValue = 0
        };
    private:
      uint16 _value;
    public:
      UInt16();
      UInt16(uint16 const&);
      ~UInt16();
      UInt16& operator = (uint16 const&);
      operator uint16 const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }
