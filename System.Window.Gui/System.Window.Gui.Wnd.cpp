// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Wnd.h"
#include "System.Exception.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
  // Default constructor
  Wnd::Wnd(HWND h)
    :_hwnd(h)
    {
    }
  // Copy constructor
  Wnd::Wnd(const Wnd& h)
    :_hwnd(h._hwnd)
    {
    }
  // Destructor
  Wnd::~Wnd()
    {
    }
  // Assignment operator for HWND
  Wnd& Wnd::operator = (HWND h)
    {
    if(_hwnd == h)
      return *this;
    _hwnd = h;
    return *this;
    }
  // Assignment operator for Wnd class
  Wnd& Wnd::operator = (const Wnd& h)
    {
    if(this == &h)
      return *this;
    _hwnd = h._hwnd;
    return *this;
    }
  // Create window
  void Wnd::CreateEx(HINSTANCE inst, LPCWSTR classname, ulong style, LPCWSTR windowname
                          ,void* param, int x, int y, int width, int height, HWND parent
                          ,HMENU menu, ulong exstyle)
    {
    _hwnd = ::CreateWindowEx(exstyle, classname, windowname, style, x, y, width, height, parent, menu, inst, param);
    if(IsNull())
      throw System::WinException(L"Failed to create window"); 
    }
  // Destroy window
  void Wnd::Destroy()
    {
    ::DestroyWindow(_hwnd);
    }
  // Get client rect
  void Wnd::GetClientRect(sys::rect& r)
    {
    ::GetClientRect(_hwnd, (RECT*)&r);
    }
  // Invalidate window rectangle
  void Wnd::InvalidateRect(RECT* rect, BOOL erase)
    {
    ::InvalidateRect(_hwnd, rect, erase);
    }
  // Send message
  intptr Wnd::SendMessage(uint32 msg, WPARAM wparam, LPARAM lparam)
    {
    return ::SendMessage(_hwnd, msg, wparam, lparam);
    }
  // Set text
  void Wnd::SetText(System::String& s)
    {
    ::SetWindowText(_hwnd, (cstring)s);
    }
  // Set window position
  void Wnd::SetWindowPos(long x, long y, long width, long height, int flags, HWND insertAfter)
    {
    ::SetWindowPos(_hwnd, insertAfter, x, y, width, height, flags);
    }
  // Show window
  void Wnd::Show(int s)
    {
    ::ShowWindow(_hwnd, s);
    }
  // Update window
  void Wnd::Update()
    {
    ::UpdateWindow(_hwnd);
    }
  }
}
}
