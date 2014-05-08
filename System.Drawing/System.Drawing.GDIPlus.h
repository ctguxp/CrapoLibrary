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
        ulong                                  _gdiplusToken;
        AutoPtr<Gdiplus::GdiplusStartupInput>  _startInput;
        AutoPtr<Gdiplus::GdiplusStartupOutput> _startOutput;
      };
    }
  }

