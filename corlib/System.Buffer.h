// Perch Lake Computer System

#pragma once
#include "System.Array.h"

namespace System
  {
  class CRAPOCOREDLL_API Buffer
    {
    public:
      Buffer();
      ~Buffer();
      static bool BlockCopy(ByteArray& src, int src_offset, ByteArray& dest, int dest_offset, int count);
    };
  }

