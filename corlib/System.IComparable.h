// Perch Lake Computer System

#pragma once
#include "System.Object.h"

namespace System
  {
  class IComparable
    {
    public:
      virtual int CompareTo(Object& obj) = 0;
    };  

  template<class T>
  class IComparableT
    {
    public:
      virtual int CompareTo(T&) = 0;
    };
  }
