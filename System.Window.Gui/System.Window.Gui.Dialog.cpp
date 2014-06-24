// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Dialog.h"
#include "System.Exception.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default constructor
      Dialog::Dialog(HINSTANCE inst, int id, DialogController* p)
        :_instance(inst)
        ,_templateName(MAKEINTRESOURCE(id))
        ,_hwndParent(NULL)
        ,_dialogFunc(DialogProc)
        ,_controller(p)
        ,_hwnd()
        {
        _controller->Type(DialogController::HandleType::Modeless);
        }
      // Destructor
      Dialog::~Dialog()
        {
        }
      // Destructor
      void Dialog::Create()
        {
        _hwnd = ::CreateDialogParam(_instance, _templateName, _hwndParent, _dialogFunc, (LPARAM)_controller);
        if(_hwnd.IsNull())
          throw System::WinException(L"Failed to create dialog");
        }
      }
    }
  }
