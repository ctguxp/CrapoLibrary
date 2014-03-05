// Perch Lake Computer System

#pragma once
#include "System.Object.h"

namespace System
  {
  class IFormatProvider
    {
    public:
    virtual Object* GetFormat(Object*) = 0;
    };
  }
