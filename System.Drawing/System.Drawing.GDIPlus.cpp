#include "pch.h"
#include "System.Drawing.GDIPlus.h"

namespace System
  {
  using namespace Runtime;
  namespace Drawing
    {

    ULONG_PTR GDIPlus::_gdiplusToken = 0;
    AutoPtr<GdiplusStartupInput>        GDIPlus::_startInput(new GdiplusStartupInput());
    AutoPtr<GdiplusStartupOutput>       GDIPlus::_startOutput(new GdiplusStartupOutput());
    AutoPtr<InteropServices::DllImport> GDIPlus::_gdiDll(new InteropServices::DllImport(L"gdiplus.dll"));

    GDIPlus::GDIPlus()
      {
      _startInput->GdiplusVersion = 1;
      _startInput->DebugEventCallback = nullptr;
      _startInput->SuppressBackgroundThread = FALSE;
      _startInput->SuppressExternalCodecs = FALSE;

      _startOutput->NotificationHook = nullptr;
      _startOutput->NotificationUnhook = nullptr;

      Startup startup;
      _gdiDll->GetFunction(L"GdiplusStartup", startup);
      startup(&_gdiplusToken, _startInput.Get(), _startOutput.Get());
      }
    GDIPlus::~GDIPlus()
      {
      Shutdown shutdown;
      _gdiDll->GetFunction(L"GdiplusShutdown", shutdown);
      shutdown(_gdiplusToken);
      }
    ULONG_PTR GDIPlus::GdiPlusToken()
      {
      return GDIPlus::_gdiplusToken;
      }
    int GDIPlus::GdipCreateFromHDC(HDC hdc, void** graphics)
      {
      CreateFromHDC create;
      _gdiDll->GetFunction(L"GdipCreateFromHDC", create);
      return create(hdc, graphics);
      }
    int GDIPlus::GdipCreatePen1(DWORD argb, float width, int unit, void** pen)
      {
      CreatePen1 pen1;
      _gdiDll->GetFunction(L"GdipCreatePen1", pen1);
      return pen1(argb, width, unit, pen);
      }
    int GDIPlus::GdipDeleteGraphics(void* graphics)
      {
      DeleteGraphics delGraphics;
      _gdiDll->GetFunction(L"GdipDeleteGraphics", delGraphics);
      return delGraphics(graphics);
      }
    int GDIPlus::GdipDeletePen(void* pen)
      {
      DeletePen delPen;
      _gdiDll->GetFunction(L"GdipDeletePen", delPen);
      return delPen(pen);
      }
    int GDIPlus::GdipDrawLineI(void* graphics, void* pen, int x1, int y1, int x2, int y2)
      {
      DrawLineI draw;
      _gdiDll->GetFunction(L"GdipDrawLineI", draw);
      return draw(graphics, pen, x1, y1, x2, y2);
      }
    int GDIPlus::GdipGetImageHeight(void* image, UINT* height)
      {
      GetHeight getHeight;
      _gdiDll->GetFunction(L"GdipGetImageHeight", getHeight);
      return getHeight(image, height);
      }
    int GDIPlus::GdipGetImageWidth(void* image, UINT* width)
      {
      GetWidth getWidth;
      _gdiDll->GetFunction(L"GdipGetImageWidth", getWidth);
      return getWidth(image, width);
      }
    int GDIPlus::GdipCreateBitmapFromScan0(INT width, INT height, INT stride, INT format, BYTE* scan0, void** bitmap)
      {
      CreateBitmapFromScan0 createBitmap;
      _gdiDll->GetFunction(L"GdipCreateBitmapFromScan0", createBitmap);
      return createBitmap(width, height, stride, format, scan0, bitmap);
      }
    int GDIPlus::GdipDisposeImage(void* image)
      {
      DisposeImage disposeImage;
      _gdiDll->GetFunction(L"GdipDisposeImage", disposeImage);
      return disposeImage(image);
      }
    int GDIPlus::GdipGetImagePixelFormat(void* image, INT* format)
      {
      GetImagePixelFormat getImagePixelFormat;
      _gdiDll->GetFunction(L"GdipGetImagePixelFormat", getImagePixelFormat);
      return getImagePixelFormat(image, format);
      }
    int GDIPlus::GdipGetImageGraphicsContext(void* image, void** graphics)
      {
      GetImageGraphicsContext getImageGraphicsContext;
      _gdiDll->GetFunction(L"GdipGetImageGraphicsContext", getImageGraphicsContext);
      return getImageGraphicsContext(image, graphics);
      }
    int GDIPlus::GdipDrawImageI(void* graphics, void* image, INT x, INT y)
      {
      DrawImageI drawImage;
      _gdiDll->GetFunction(L"GdipDrawImageI", drawImage);
      return drawImage(graphics, image, x, y);
      }
    int GDIPlus::GdipCreateBitmapFromFile(LPCWSTR file, void** bitmap)
      {
      CreateBitmapFromFile createFromFile;
      _gdiDll->GetFunction(L"GdipCreateBitmapFromFile", createFromFile);
      return createFromFile(file, bitmap);
      }
    int GDIPlus::GdipDrawImageRectI(void* graphics, void* image, INT x, INT y, INT width, INT height)
      {
      DrawImageRectI drawImage;
      _gdiDll->GetFunction(L"GdipDrawImageRectI", drawImage);
      return drawImage(graphics, image, x, y, width, height);
      }
    int GDIPlus::GdipBitmapGetPixel(void* bitmap, INT x, INT y, DWORD* color)
      {
      BitmapGetPixel bitmapGetPixel;
      _gdiDll->GetFunction(L"GdipBitmapGetPixel", bitmapGetPixel);
      return bitmapGetPixel(bitmap, x, y, color);
      }
    }
  }