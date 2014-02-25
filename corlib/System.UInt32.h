#pragma once
#include <stdint.h>

namespace System
  {
  class UInt32
    {
    public:
      enum : uint32
        {
        MaxValue = 0xffffffffui32,
        MinValue = 0
        };
    public:
      UInt32();
      ~UInt32();
    };
  }
