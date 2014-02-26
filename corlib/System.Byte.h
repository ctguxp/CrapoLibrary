#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API Byte : public Object
    {
    public: 
      enum : byte
        {
        MinValue = 0,
        MaxValue = 255
        };
    private:
      byte _value;
    public:
      Byte();
      Byte(byte const&);
      ~Byte();
      Byte& operator = (byte const& value);
      operator byte const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }

