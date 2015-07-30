#include "pch.h"
#include "System.Drawing.Image.h"
#include "System.Drawing.GDIPlus.h"

namespace System
  {
  namespace Drawing
    {
    Image::Image()
      :_nativeObject(nullptr)
      {
      }
    Image::~Image()
      {
      if (GDIPlus::GdiPlusToken() != 0 && _nativeObject != nullptr)
        {
        /*int status = */GDIPlus::GdipDisposeImage(_nativeObject);
        // dispose the stream (set under Win32 only if SD owns the stream) and ...
        /*if (stream != null)
        {
        stream.Close ();
        stream = null;
        }*/
        // ... set nativeObject to null before (possibly) throwing an exception
        _nativeObject = nullptr;
        //GDIPlus.CheckStatus (status);
        }	
      }
    Imaging::PixelFormat Image::PixelFormat()
      {
      INT result;
      /*int status = */GDIPlus::GdipGetImagePixelFormat(_nativeObject, &result);
      //GDIPlus.CheckStatus (status);
      return (Imaging::PixelFormat)result;
      }
    int Image::Height()
      {
      UINT height;			
      /*int status = */GDIPlus::GdipGetImageWidth(_nativeObject, &height);		
      //GDIPlus.CheckStatus (status);			
      return (int)height;
      }
    int Image::Width()
      {
      UINT width;			
      /*int status = */GDIPlus::GdipGetImageWidth(_nativeObject, &width);		
      //GDIPlus.CheckStatus (status);			
      return (int)width;
      }
    }
  }
