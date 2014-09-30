#pragma once
#include "System.Drawing.KnownColor.h"

namespace System
  {
  namespace Drawing
    {
    class Color;
    class KnownColors
      {
      public:
        KnownColors();
        ~KnownColors();
        static Color FromKnownColor(KnownColor kc);
      };
    }
  }
