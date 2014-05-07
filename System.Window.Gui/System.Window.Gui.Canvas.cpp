#include "pch.h"
#include "System.Window.Gui.Canvas.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {

      Canvas::Canvas(HDC hdc)
        :_hdc(hdc)
        {
        }
      Canvas::~Canvas()
        {
        }

      PaintCanvas::PaintCanvas(HWND hwnd)
        :Canvas(::BeginPaint(hwnd, &_ps))
        ,_hwnd(hwnd)
        {
        }
      PaintCanvas::~PaintCanvas()
        {
        ::EndPaint(_hwnd, &_ps);
        }

      UpdateCanvas::UpdateCanvas(HWND hwnd)
        :Canvas(::GetDC(hwnd))
        ,_hwnd(hwnd)
        {
        }
      
      UpdateCanvas::~UpdateCanvas()
        {
        ::ReleaseDC(_hwnd, _hdc);
        }

      }
    }
  }
