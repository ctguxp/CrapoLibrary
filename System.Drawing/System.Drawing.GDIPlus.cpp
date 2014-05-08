#include "pch.h"
#include "System.Drawing.GDIPlus.h"

using namespace Gdiplus;

namespace System
  {
  namespace Drawing
    {
    GDIPlus::GDIPlus()
      :_gdiplusToken()
      ,_startInput()
      ,_startOutput()
      {
      _startInput.Set(new GdiplusStartupInput());
      _startInput->GdiplusVersion = 1;
      _startInput->DebugEventCallback = nullptr;
      _startInput->SuppressBackgroundThread = FALSE;
      _startInput->SuppressExternalCodecs = FALSE;

      _startOutput.Set(new GdiplusStartupOutput());
      _startOutput->NotificationHook = nullptr;
      _startOutput->NotificationUnhook = nullptr;

      GdiplusStartup(&_gdiplusToken, _startInput.Get(), _startOutput.Get());
      }
    GDIPlus::~GDIPlus()
      {
      GdiplusShutdown(_gdiplusToken);
      }
    }
  }