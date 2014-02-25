#pragma once
#include <stdint.h>

namespace System
  {
  class CRAPOCOREDLL_API UInt32
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
