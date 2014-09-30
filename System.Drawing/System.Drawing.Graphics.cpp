#include "pch.h"
#include "System.Drawing.Graphics.h"
#include "System.Drawing.GDIPlus.h"
#include "System.Drawing.Image.h"
#include "System.Drawing.Imaging.PixelFormat.h"

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
    Graphics* Graphics::FromImage(Image* image)
      {
      void* graphics;
      if(image == nullptr)
        throw ArgumentException(L"image");

      if( ((intptr)image->PixelFormat() & (intptr)Imaging::PixelFormat::Indexed) != 0)
        throw Exception(L"Cannot create graphics from an indexed bitmap.");

      int status = GDIPlus::GdipGetImageGraphicsContext(image->NativeObject(), &graphics);
      //GDIPlus.CheckStatus (status);
      return new Graphics(graphics);
      }
    void Graphics::DrawImage(Image* image, int x, int y)
		{
			if(image == nullptr)
				throw ArgumentNullException(L"image");
			int status = GDIPlus::GdipDrawImageI(_nativeObject, image->NativeObject(), x, y);
			//GDIPlus.CheckStatus (status);
		}
    void Graphics::DrawImage(Image* image, int x, int y, int width, int height)
		{
			if(image == nullptr)
				throw ArgumentNullException(L"image");
			int status = GDIPlus::GdipDrawImageRectI(_nativeObject, image->NativeObject(), x, y, width, height);
			//GDIPlus.CheckStatus (status);
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
