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
    }
  }