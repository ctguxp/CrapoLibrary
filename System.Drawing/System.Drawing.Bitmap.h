#pragma once
#include "System.Drawing.Image.h"
#include "System.Drawing.Imaging.PixelFormat.h"

namespace System
  {
  namespace Drawing
    {
    class Color;
    class Bitmap : public Image
      {
      public:
        Bitmap(int /*width*/, int /*height*/, Imaging::PixelFormat format = Imaging::PixelFormat::Format32bppPArgb);
        Bitmap(String filename, bool useIcm = false);
        Bitmap(Image* /*original*/, int width = 0, int height = 0, Imaging::PixelFormat format = Imaging::PixelFormat::Format32bppPArgb);
        ~Bitmap();
        Color GetPixel(int x, int y);
      };
    }
  }
