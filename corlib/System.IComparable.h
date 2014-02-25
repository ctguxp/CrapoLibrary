// Perch Lake Computer System

#pragma once
#include "System.Object.h"

namespace System
  {
  template<class T>
  class IComparable
    {
    virtual int CompareTo(T&) = 0;
    };
  }
