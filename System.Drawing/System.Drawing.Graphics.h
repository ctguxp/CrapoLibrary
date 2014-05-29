#pragma once
#include <AutoPtr.h>
#include "System.Drawing.Pen.h"

namespace System
  {
  namespace Drawing
    {
    class Graphics
      {
      public:
        Graphics(void* /*nativGraphics*/);
        ~Graphics();
        void DrawLine(Pen* pen, int x1, int y1, int x2, int y2);
        static Graphics* FromHdc(HDC hdc);
      private:
        void* _nativeObject;
      };
    typedef AutoPtr<Graphics> GCGraphics;
    }
  }
