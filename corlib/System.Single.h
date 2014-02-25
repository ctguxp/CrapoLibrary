// Perch Lake Computer System

#pragma once
#include "System.Object.h"

namespace System
  {
  class CRAPOCOREDLL_API Single : public Object
    {
    private:
      float m_value;
    public:
      Single();
      ~Single();
    public:
      static float NaN;
    };
  }

