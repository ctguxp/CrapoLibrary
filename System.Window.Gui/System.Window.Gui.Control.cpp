// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Control.h"
#include "System.Array.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default constructor
      Control::Control(int id)
        :_id(id)
        ,_hwnd()
        {
        }
      // Destructor
      Control::~Control()
        {
        }
      // Create control
      void Control::Create(HWND parent)
        {
        _hwnd = ::GetDlgItem(parent, _id);
        }
      void Control::Enable()
        {
        _hwnd.Enable();
        }
      void Control::Disable()
        {
        _hwnd.Disable();
        }
      // Get text
      String Control::Text()
        {
        intptr buflen = _hwnd.SendMessage(WM_GETTEXTLENGTH);
        CharArray buffer(buflen + 1);
        /*intptr retval = */_hwnd.SendMessage(WM_GETTEXT, (WPARAM)buflen + 1, (LPARAM)buffer.ToPtr());
        String s(buffer.ToConstPtr());
        return s;
        }
      void Control::Text(String value)
        {
        /*intptr retval = */_hwnd.SendMessage(WM_SETTEXT, 0, (LPARAM)(cstring)value);
        }
      }
    }
  }
