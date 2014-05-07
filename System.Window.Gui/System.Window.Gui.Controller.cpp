// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Controller.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Base Controller class

      // Default constructor
      Controller::Controller()
        :_hwnd()
        ,_instance(NULL)
        {
        }
      // Destructor
      Controller::~Controller()
        {
        }
      // On destroy handler
      bool Controller::OnDestroy(int exitCode)
        {
        ::PostQuitMessage(exitCode);
        return true;
        }
      // On close handler
      bool Controller::OnClose()
        {
        _hwnd.Destroy();
        return true;
        }

      // Window Controller class

      // Default constructor
      WinController::WinController()
        {
        }
      // Destructor
      WinController::~WinController()
        {
        }

      // Dialog Controller class

      // Default constructor
      DialogController::DialogController()
        {
        }
      // Destructor
      DialogController::~DialogController()
        {
        }
      }
    }
  }
