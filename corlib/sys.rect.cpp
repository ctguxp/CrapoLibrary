// Perch Lake Computer System

#include "pch.h"
#include "sys.rect.h"

namespace sys
  {
  // Default constructor
  rect::rect(long left, long top, long right, long bottom)
    :_left(left)
    ,_top(top)
    ,_right(right)
    ,_bottom(bottom)
    {
    }
  // Destructor
  rect::~rect()
    {
    }
  }
