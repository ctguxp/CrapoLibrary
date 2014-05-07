// Perch Lake Computer System

#pragma once
#include "System.String.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default window callback procedure for all windows
      LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

      // Retrieves information about the specified window
      template<class T>
      T GetWinLong(HWND h, int i = GWLP_USERDATA)
        {
        return (T)::GetWindowLongPtr(h, i);
        }

      // Changes an attribute of the specified window
      template<class T>
      void SetWinLong(HWND h, T d, int i = GWLP_USERDATA)
        {
        ::SetWindowLongPtr(h, i, (LONG_PTR)d);
        }

      class WinClass
        {
        public:
          // Default constructor
          WinClass(HINSTANCE inst, LPCWSTR className);
          // Destructor
          ~WinClass();
          // Register class
          void Register();
          // Set menu property
          void Menu(int value) { _menuName = MAKEINTRESOURCE(value); }
        private:
          uint32      _size;        // The size, in bytes, of this structure
          uint32      _style;       // The class style(s)
          WNDPROC   _wndProc;       // A pointer to the window procedure
          int       _clsExtra;      // The number of extra bytes to allocate following the window-class structure
          int       _wndExtra;      // The number of extra bytes to allocate following the window instance
          HINSTANCE _instance;      // A handle to the instance that contains the window procedure for the class
          HICON     _icon;          // A handle to the class icon
          HCURSOR   _cursor;        // A handle to the class cursor
          HBRUSH    _background;    // A handle to the class background brush
          LPCWSTR   _menuName;      // Pointer to a null-terminated character string that specifies the resource name of the class menu
          LPCWSTR   _className;     // A pointer to a null-terminated string
          HICON     _iconSm;        // A handle to a small icon that is associated with the window class
        };
      }
    }
  }
