#pragma once
#include <AutoPtr.h>
#include "System.Drawing.Pen.h"

namespace System
  {
  namespace Drawing
    {
    class Image;
    class Graphics
      {
      public:
        Graphics(void* /*nativGraphics*/);
        ~Graphics();
        void DrawImage(Image* image, int x, int y);
        void DrawImage(Image* image, int x, int y, int width, int height);
        void DrawLine(Pen* pen, int x1, int y1, int x2, int y2);
        static Graphics* FromHdc(HDC hdc);
        static Graphics* FromImage(Image* image);
      private:
        void* _nativeObject;
      };
    typedef AutoPtr<Graphics> GCGraphics;
    }
  }
