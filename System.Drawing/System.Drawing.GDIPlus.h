#pragma once
#include "System.Runtime.InteropServices.DllImport.h"

namespace System
  {
  namespace Drawing
    {
    class GDIPlus
      {
      private:
        static ULONG_PTR                                    _gdiplusToken;
        static AutoPtr<GdiplusStartupInput>                 _startInput;
        static AutoPtr<GdiplusStartupOutput>                _startOutput;
        static AutoPtr<Runtime::InteropServices::DllImport> _gdiDll;
      public:
        GDIPlus();
        ~GDIPlus();
        static ULONG_PTR GdiPlusToken();
        static int GdipCreateFromHDC(HDC /*hdc*/, void** /*graphics*/);
        static int GdipCreatePen1(DWORD argb, float width, int unit, void** pen);
        static int GdipDeleteGraphics(void* /*graphics*/);
        static int GdipDeletePen(void* /*pen*/);
        static int GdipDrawLineI(void* /*graphics*/, void* /*pen*/, int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/);
        static int GdipGetImageHeight(void* /*image*/, UINT* /*height*/);
        static int GdipGetImageWidth(void* /*image*/, UINT* /*width*/);
        static int GdipCreateBitmapFromScan0(INT /*width*/, INT /*height*/, INT /*stride*/, INT /*format*/, BYTE* /*scan0*/, void** /*bitmap*/);
        static int GdipDisposeImage(void* /*image*/);
        static int GdipGetImagePixelFormat(void* /*image*/, INT* /*format*/);
        static int GdipGetImageGraphicsContext(void* /*image*/, void** /*graphics*/);
        static int GdipDrawImageI(void* /*graphics*/, void* /*image*/, INT /*x*/, INT /*y*/);
        static int GdipCreateBitmapFromFile(LPCWSTR /*file*/, void** /*bitmap*/);
        static int GdipDrawImageRectI(void* /*graphics*/, void* /*image*/, INT /*x*/, INT /*y*/, INT /*width*/, INT /*height*/);
        static int GdipBitmapGetPixel(void* /*bitmap*/, INT /*x*/, INT /*y*/, DWORD* /*color*/);
      private:
        typedef int (FAR PASCAL* Startup)(ULONG_PTR* token, const GdiplusStartupInput* input, GdiplusStartupOutput* output);
        typedef void (FAR PASCAL* Shutdown)(ULONG_PTR token);
        typedef int (FAR PASCAL* CreateFromHDC)(HDC hdc, void** graphics);
        typedef int (FAR PASCAL* CreatePen1)(DWORD color, float width, int unit, void **pen);
        typedef int (FAR PASCAL* DeleteGraphics)(void* graphics);
        typedef int (FAR PASCAL* DeletePen)(void* pen);
        typedef int (FAR PASCAL* DrawLineI)(void* graphics, void* pen, int x1, int y1, int x2, int y2);
        typedef int (FAR PASCAL* GetHeight)(void* image, UINT* height);
        typedef int (FAR PASCAL* GetWidth)(void* image, UINT* width);
        typedef int (FAR PASCAL* CreateBitmapFromScan0)(INT width, INT height, INT stride, INT format, BYTE* scan0, void** bitmap);
        typedef int (FAR PASCAL* DisposeImage)(void* image);
        typedef int (FAR PASCAL* GetImagePixelFormat)(void* image, INT* format);
        typedef int (FAR PASCAL* GetImageGraphicsContext)(void* image, void** graphics);
        typedef int (FAR PASCAL* DrawImageI)(void* graphics, void* image, INT x, INT y);
        typedef int (FAR PASCAL* CreateBitmapFromFile)(LPCWSTR file, void** bitmap);
        typedef int (FAR PASCAL* DrawImageRectI)(void* graphics, void* image, INT x, INT y, INT width, INT height);
        typedef int (FAR PASCAL* BitmapGetPixel)(void* bitmap, INT x, INT y, DWORD* color);
      };
    }
  }

