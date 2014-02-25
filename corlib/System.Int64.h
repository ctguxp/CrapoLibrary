#pragma once
#include <stdint.h>

namespace System
  {
  class Int64
    {
    public:
    enum : int64
      {
      MaxValue = 9223372036854775807i64,
      MinValue = (-9223372036854775807i64 - 1)
      };
    public:
      Int64();
      ~Int64();
    };
  }
