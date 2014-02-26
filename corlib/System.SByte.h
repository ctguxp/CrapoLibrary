#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API SByte : public Object
    {
    public: 
      enum : sbyte
        {
        MinValue = (-127i8 - 1),
        MaxValue = 127
        };
    private:
      sbyte _value;
    public:
      SByte();
      SByte(sbyte const&);
      ~SByte();
      SByte& operator = (sbyte const& value);
      operator sbyte const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }

