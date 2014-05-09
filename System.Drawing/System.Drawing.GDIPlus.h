#pragma once

namespace System
  {
  namespace Drawing
    {
    class GDIPlus
      {
      public:
        GDIPlus();
        ~GDIPlus();
      protected:
        ULONG_PTR                              _gdiplusToken;
        AutoPtr<Gdiplus::GdiplusStartupInput>  _startInput;
        AutoPtr<Gdiplus::GdiplusStartupOutput> _startOutput;
      };
    }
  }

