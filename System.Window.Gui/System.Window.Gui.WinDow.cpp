// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.WinDow.h"
#include "System.Exception.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Base window class
      // Default constructor
      WinDow::WinDow(HINSTANCE inst, LPCWSTR className)
        :_initparam(nullptr)
        ,_instance(inst)
        ,_menu(NULL)
        ,_parent(NULL)
        ,_height(0)
        ,_width(0)
        ,_y(0)
        ,_x(0)
        ,_style(0)
        ,_extstyle(0)
        ,_class(className)
        ,_name(NULL)
        ,_hwnd()
        {
        }
      // Destructor
      WinDow::~WinDow()
        {
        }
      // Create Window
      void WinDow::Create()
        {
        _hwnd.CreateEx(_instance, _class, _style, _name, _initparam, _x, _y, _width, _height
          ,_parent, _menu, _extstyle);
        }
      // Show window
      void WinDow::Show()
        {
        _hwnd.Show();
        _hwnd.Update();
        }

      // TopWinDow class (Main Window of Application)

      // Default constructor
      TopWinDow::TopWinDow(HINSTANCE inst, LPCWSTR className, LPCWSTR title, WinController* p)
        :WinDow(inst, className)
        {
        _initparam = (void*)p;
        _height = CW_USEDEFAULT;
        _width = CW_USEDEFAULT;
        _y = CW_USEDEFAULT;
        _x = CW_USEDEFAULT;
        _style = WS_OVERLAPPEDWINDOW;
        _name = title;
        }
      // Destructor
      TopWinDow::~TopWinDow()
        {
        }
      }
    }
  }