// Perch Lake Computer System

#pragma once
#include "sys.rect.h"
#include "System.String.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      class Wnd
        {
        public:
          // Default constructor
          Wnd(HWND h = NULL);
          // Copy constructor
          Wnd(const Wnd& h);
          // Assignment operator for HWND
          Wnd& operator = (HWND h);
          // Assignment operator for Wnd class
          Wnd& operator = (const Wnd& h); 
          // HWND operator
          operator HWND() { return _hwnd; }
          // Destructor
          ~Wnd();
          // Create window
          void CreateEx(HINSTANCE inst, LPCWSTR classname, ulong style, LPCWSTR windowname = NULL
            ,void* param = nullptr, int x = 0, int y = 0, int width = 0, int height = 0
            ,HWND parent = NULL, HMENU menu = NULL, ulong exstyle = 0);
          // Destroy window
          void Destroy();
          void Enable();
          void Disable();
          // Get client rect of window
          void GetClientRect(sys::rect& r);
          // Is null handle
          bool IsNull() { return _hwnd == NULL; }
          // Invalidate window rectangle
          void InvalidateRect(RECT* rect = nullptr, BOOL erase = TRUE);
          // Send message to window
          intptr SendMessage(uint32 msg, WPARAM wparam = 0, LPARAM lparam = 0);
          // Set text
          void SetText(System::String& s);
          // Set window position
          void SetWindowPos(long x, long y, long width, long height, int flags, HWND insertAfter = NULL);
          // Show window
          void Show(int s = SW_SHOWDEFAULT);
          // Update window
          void Update();
        protected:
          HWND _hwnd; // Window handle
        };
      }
    }
  }
