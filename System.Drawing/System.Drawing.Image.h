#pragma once
#include "System.Drawing.Imaging.PixelFormat.h"

namespace System
  {
  namespace Drawing
    {
    class Image
      {
      public:
        Image();
        virtual ~Image();
        int Height();
        int Width();
        Imaging::PixelFormat PixelFormat();
        void* NativeObject() { return _nativeObject; }
      protected:
        void* _nativeObject;
      };
    }
  }

