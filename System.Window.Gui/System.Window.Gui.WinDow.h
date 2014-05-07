// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Window.Gui.Wnd.h"
#include "System.Window.Gui.Controller.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      class WinDow
        {
        public:
          // Default constructor
          WinDow(HINSTANCE inst, LPCWSTR className);
          // Destructor
          ~WinDow();
          operator HWND() { return _hwnd; }
          // Create window
          void Create();
          // Show window
          void Show();
          // Set extended style property
          void ExtStyle(ulong value) { _extstyle = value; }
          // Set style property
          void Style(ulong value) { _style = value; }
          // Set parent handle property
          void Parent(HWND value) { _parent = value; }
          // Set x coordinate property
          void X(int value) { _x = value; }
          // Set y coordinate property
          void Y(int value) { _y = value; }
          // Set width property
          void Width(int value) { _width = value; }
          // Set height property
          void Height(int value) { _height = value; }
          // Set menu or id property
          void Menu(HMENU value) { _menu = value; }
        protected:
          void*           _initparam;       // Init parameter
          HINSTANCE       _instance;        // A handle to the instance of the module to be associated with the window
          HMENU           _menu;            // A handle to a menu, or specifies a child-window identifier, depending on the window style
          HWND            _parent;          // A handle to the parent or owner window of the window being created
          int             _height;          // The height, in device units, of the window
          int             _width;           // The width, in device units, of the window
          int             _y;               // The initial vertical position of the window
          int             _x;               // The initial horizontal position of the window
          ulong           _style;           // The style of the window being created
          ulong           _extstyle;        // The extended window style of the window being created
          LPCWSTR         _class;           // Permament string of class name
          LPCWSTR         _name;            // Permanent string of window name
          Wnd            _hwnd;            // Window handle object
        };

      class TopWinDow : public WinDow
        {
        public:
          // Default constructor
          TopWinDow(HINSTANCE inst, LPCWSTR className, LPCWSTR title, WinController* p);
          // Destructor
          ~TopWinDow();
        };
      }
    }
  }

