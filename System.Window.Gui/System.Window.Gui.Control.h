// Perch Lake Computer System

#pragma once
#include "System.Window.Gui.Wnd.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      class Control
        {
        public:
          // Default constructor
          Control(int id);
          // Destructor
          ~Control();
          // Create control
          void Create(HWND parent);
          // Get handle property
          Wnd& Hwnd() { return _hwnd; }
          void Enable();
          void Disable();
          String Text();
          void Text(String value);
        protected:
          int  _id;     // Resource id
          Wnd  _hwnd;   // Handle to control
        };
      }
    }
  }
