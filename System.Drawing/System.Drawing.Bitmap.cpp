#include "pch.h"
#include "System.Drawing.Bitmap.h"
#include "System.Drawing.GDIPlus.h"
#include "System.Drawing.Graphics.h"
#include "System.Drawing.Color.h"

namespace System
  {
  namespace Drawing
    {
    Bitmap::Bitmap(int width, int height, Imaging::PixelFormat format)
      {
      void* bmp;
      /*int status = */GDIPlus::GdipCreateBitmapFromScan0(width, height, 0, (INT)format, nullptr, &bmp);
      //GDIPlus.CheckStatus(s);
      _nativeObject = bmp;
      }
    Bitmap::Bitmap(Image* original, int width, int height, Imaging::PixelFormat format)
      {
      if(width == 0)
        width = original->Width();

      if(height == 0)
        height = original->Height();

      void* bmp;
      /*int status = */GDIPlus::GdipCreateBitmapFromScan0(width, height, 0, (INT)format, nullptr, &bmp);
      //GDIPlus.CheckStatus(s);
      _nativeObject = bmp;

      GCGraphics graphics(Graphics::FromImage(this));

      graphics->DrawImage(original, 0, 0, width, height);
      }
    Bitmap::Bitmap(String filename, bool /*useIcm*/)
      {
      void* imagePtr;
      /*int st;*/

      //if(useIcm)
      //st = GDIPlus::GdipCreateBitmapFromFileICM(filename, &imagePtr);
      //else
      /*st = */GDIPlus::GdipCreateBitmapFromFile(filename, &imagePtr);

      //GDIPlus.CheckStatus (st);
      _nativeObject = imagePtr;
      }
    Bitmap::~Bitmap()
      {
      }
    Color Bitmap::GetPixel(int x, int y)
      {	
      DWORD argb;				

      /*int s = */GDIPlus::GdipBitmapGetPixel(_nativeObject, x, y, &argb);
      //GDIPlus.CheckStatus (s);

      return Color::FromArgb((int)argb);		
      }
    }
  }