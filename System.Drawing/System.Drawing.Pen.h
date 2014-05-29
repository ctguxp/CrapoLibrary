#pragma once
#include "System.Drawing.Color.h"

namespace System
  {
  namespace Drawing
    {
    class Pen
      {
      public:
        Pen(Color /*color*/, float width = 1.0);
        ~Pen();
        operator void*() { return _nativeObject; }
      private:
        Color  _color;
        void* _nativeObject;
      };
    }
  }
