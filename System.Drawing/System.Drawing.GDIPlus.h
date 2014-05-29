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
        static int GdipCreateFromHDC(HDC /*hdc*/, void** /*graphics*/);
        static int GdipCreatePen1(DWORD argb, float width, int unit, void** pen);
        static int GdipDeleteGraphics(void* /*graphics*/);
        static int GdipDeletePen(void* /*pen*/);
        static int GdipDrawLineI(void* /*graphics*/, void* /*pen*/, int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/);
      private:
        typedef int (FAR PASCAL* Startup)(ULONG_PTR* token, const GdiplusStartupInput* input, GdiplusStartupOutput* output);
        typedef void (FAR PASCAL* Shutdown)(ULONG_PTR token);
        typedef int (FAR PASCAL* CreateFromHDC)(HDC hdc, void** graphics);
        typedef int (FAR PASCAL* CreatePen1)(DWORD color, float width, int unit, void **pen);
        typedef int (FAR PASCAL* DeleteGraphics)(void* graphics);
        typedef int (FAR PASCAL* DeletePen)(void* pen);
        typedef int (FAR PASCAL* DrawLineI)(void* graphics, void* pen, int x1, int y1, int x2, int y2);
      };
    }
  }

