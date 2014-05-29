#include "pch.h"
#include "System.Drawing.Graphics.h"
#include "System.Drawing.GDIPlus.h"

namespace System
  {
  namespace Drawing
    {
    Graphics::Graphics(void* nativGraphics)
      :_nativeObject(nativGraphics)
      {
      }
    Graphics::~Graphics()
      {
      int status = GDIPlus::GdipDeleteGraphics(_nativeObject);
      _nativeObject = nullptr;
      }
    Graphics* Graphics::FromHdc(HDC hdc)
      {
      void* graphics;
      int status = GDIPlus::GdipCreateFromHDC(hdc, &graphics);
      //GDIPlus.CheckStatus (status);
      return new Graphics(graphics);
      }
    void Graphics::DrawLine(Pen* pen, int x1, int y1, int x2, int y2)
      {
      if(pen == nullptr)
        throw ArgumentNullException(L"pen");
      int status = GDIPlus::GdipDrawLineI(_nativeObject, (*pen), x1, y1, x2, y2);
      //GDIPlus.CheckStatus(status);
      }
    }
  }
