// Perch Lake Computer System

#pragma once
#include "System.Object.h"

namespace System
  {
  template<class T>
  class IEquatable
    {
    virtual bool Equals(T) = 0;
    };
  }
