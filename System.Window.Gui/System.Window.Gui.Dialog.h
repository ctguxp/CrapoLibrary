// Perch Lake Computer System

#pragma once
#include "System.Window.Gui.Wnd.h"
#include "System.Window.Gui.Controller.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default callback for all dialogs
      INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

      class Dialog
        {
        public:
          // Default constructor
          Dialog(HINSTANCE inst, int id, DialogController* p);
          // Destructor
          ~Dialog();
          void Create();
          HWND Hwnd() { return _hwnd; }
        private:
          HINSTANCE         _instance;      // A handle to the module whose executable file contains the dialog
          LPCTSTR           _templateName;  // The dialog box template
          HWND              _hwndParent;    // A handle to the window that owns the dialog
          DLGPROC           _dialogFunc;    // A pointer to the dialog procedure
          DialogController* _controller;    // A pointer to the dialog controller
          Wnd               _hwnd;          // Handle of dialog
        };
      }
    }
  }
