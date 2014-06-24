// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.CommonControl.h"
#include "System.Exception.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default constructor
      CommonControl::CommonControl()
        :_size(sizeof(INITCOMMONCONTROLSEX))
        ,_icc(ICC_WIN95_CLASSES) // Load animate control, header, hot key, list-view, progress bar
        // status bar, tab, tooltip, toolbar, trackbar, tree-view
        // and up-down control classes.
        {
        }
      // Destructor
      CommonControl::~CommonControl()
        {
        }
      // Ensures that the common control DLL get loaded (Put in WinMain)
      void CommonControl::Initialize()
        {
        if(!::InitCommonControlsEx((INITCOMMONCONTROLSEX*)this))
          throw System::WinException(L"Failed to init common controls");
        }
      }
    }
  }
