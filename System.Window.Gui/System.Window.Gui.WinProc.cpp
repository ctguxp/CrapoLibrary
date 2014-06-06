// Perch Lake Computer System

#include "pch.h"
#include "System.Window.Gui.WinClass.h"
#include "System.Window.Gui.Controller.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      // Default window callback procedure for all windows
      LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
        // Get controller from window user data
        WinController* p = GetWinLong<WinController*>(hwnd);
        switch(msg)
          {   
          case WM_CLOSE:
            {
            // Handle on close
            if(p->OnClose())
              return 0;
            }
            break;
          case WM_COMMAND:
            {
            // Handle on command
            if(p->OnCommand(HIWORD(wparam), LOWORD(wparam), (HWND)lparam))
              return 0;
            }
            break;
          case WM_CREATE:
            {
            // Handle on create
            if(p->OnCreate((CREATESTRUCT*)lparam))
              return 0;
            }
            break;
          case WM_DESTROY:
            {
            // Handle on destroy
            if(p->OnDestroy())
              return 0;
            }
            break;
          case WM_NCCREATE:
            {
            // Derive CREATESTRUCT from lparam
            CREATESTRUCT* cs = (CREATESTRUCT*)lparam;
            // Derive controller from CREATESTRUCT's lpCreateParams
            p = (WinController*)cs->lpCreateParams;
            p->Hwnd(hwnd); // Set controllers handle

            // Get instance from window
            HINSTANCE h = GetWinLong<HINSTANCE>(hwnd, GWLP_HINSTANCE);
            p->Instance(h); // Set controllers instance

            // Set dialog controller to windows user data
            SetWinLong<WinController*>(hwnd, p);
            }
            break;
          case WM_PAINT:
            {
            if(p->OnPaint())
              return 0;
            }
            break;
          case WM_SIZE:
            {
            // Handle on size
            if(p->OnSize((uint32)wparam, LOWORD(lparam), HIWORD(lparam)))
              return 0;
            }
            break;
          }
        // Default handling of messages
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
      }
    }
  }
