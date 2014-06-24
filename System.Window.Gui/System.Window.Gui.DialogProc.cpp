// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.Dialog.h"
#include "System.Window.Gui.Controller.h"
#include "System.Window.Gui.WinClass.h"

#pragma warning(disable:4100)

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default callback for all dialogs
      INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
        // Get DialogController from window user data
        DialogController* p = GetWinLong<DialogController*>(hwnd);

        switch(msg)
          {
          case WM_INITDIALOG:
            {
            // Derive dialog controller from lparam
            p = (DialogController*)lparam;
            p->Hwnd(hwnd);    // Set controllers handle

            // Get instance from window
            HINSTANCE h = GetWinLong<HINSTANCE>(hwnd, GWLP_HINSTANCE);
            p->Instance(h); // Set controllers instance

            // Set DialogController to windows user data
            SetWinLong<DialogController*>(hwnd, p);

            // Handle on initialize
            if(p->OnInit())
              return TRUE;
            }
            break;
          case WM_CLOSE:
            {
            // Handle on close
            if(p->OnClose())
              return TRUE;
            }
            break;
          case WM_COMMAND:
            {
            // Handle on command (code, id, hwnd)
            if(p->OnCommand(HIWORD(wparam), LOWORD(wparam), (HWND)lparam))
              return TRUE;
            }
            break;
          case WM_DESTROY:
            {
            // Handle on destroy
            if(!p->IsModal())  // Modal dialogs should not go here
              if(p->OnDestroy())
                return TRUE;
            }
            break;
          }
        return FALSE;
        }
      }
    }
  }

#pragma warning(default:4100)
