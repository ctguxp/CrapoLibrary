// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Msg.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      { 
      // Default constructor
      Msg::Msg()
        :_hwnd(NULL)
        ,_message(0)
        ,_wparam(0)
        ,_lparam(0)
        ,_time(0)
        {
        _pt.x = 0;
        _pt.y = 0;
        }
      // Destructor
      Msg::~Msg()
        {
        }
      // Dispatch message
      void Msg::Dispatch()
        {
        ::DispatchMessage((MSG*)this);
        }
      // Get message
      BOOL Msg::Get(HWND hwnd, uint32 msgFilterMin, uint32 msgFilterMax)
        {
        return ::GetMessage((MSG*)this, hwnd, msgFilterMin, msgFilterMax);
        }
      // Is dialog
      BOOL Msg::IsDialog(HWND hwnd)
        {
        return ::IsDialogMessage(hwnd, (MSG*)this);
        }  
      // Translate message
      void Msg::Translate()
        {
        ::TranslateMessage((MSG*)this);
        }
      }
    }
  }