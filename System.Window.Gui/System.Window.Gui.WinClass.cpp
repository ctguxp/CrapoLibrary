// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.WinClass.h"
#include "System.Exception.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default constructor
      WinClass::WinClass(HINSTANCE inst, LPCWSTR className)
        :_size(sizeof(WNDCLASSEXW))
        ,_style(0)
        ,_wndProc(WinProc)
        ,_clsExtra(0)
        ,_wndExtra(0)
        ,_instance(inst)
        ,_icon(NULL)
        ,_cursor(NULL)
        ,_background((HBRUSH)(COLOR_WINDOW + 1))
        ,_menuName(NULL)
        ,_className(className)
        ,_iconSm(NULL)
        {
        }
      // Destructor
      WinClass::~WinClass()
        {
        }
      // Register Class
      void WinClass::Register()
        {
        ATOM a = ::RegisterClassEx((WNDCLASSEXW*)this);
        if(a == 0)
          throw System::WinException(L"Failed to register class");
        }
      }
    }
  }