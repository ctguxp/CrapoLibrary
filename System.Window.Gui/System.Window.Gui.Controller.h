// Perch Lake Computer System

#pragma once
#include "System.Window.Gui.Wnd.h"

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      class Controller
        {
        public:
          // Destructor
          virtual ~Controller();
          // Get handle property
          HWND Hwnd() { return _hwnd; }
          // Set handle property
          void Hwnd(HWND value) { _hwnd = value; }
          // Set instance property
          void Instance(HINSTANCE value) { _instance = value; }
          // On destroy handler
          virtual bool OnDestroy(int exitCode = 0);
          // On close handler
          virtual bool OnClose();
          // On command handler
          virtual bool OnCommand(uint16 /*code*/, uint16 /*id*/, HWND /*hwnd*/) { return false; }
          // On size handler
          virtual bool OnSize(uint32, long, long) { return false; }
        protected:
          // Default constructor
          Controller();
        protected:
          Wnd       _hwnd;      // Handle to window
          HINSTANCE _instance;  // Instance
        };

      class WinController : public Controller
        {
        public:
          // Default constructor
          WinController();
          // Destructor
          ~WinController();
          // On create handler
          virtual bool OnCreate(CREATESTRUCT*) { return false; }
          virtual bool OnPaint() { return false; }
        };


      // Dialog controller class
      class DialogController : public Controller
        {
        public:
          // Handle type enumeration
          enum class HandleType
            {
            Modal = 0,
            Modeless = 1
            };
        public:
          // Default constructor
          DialogController();
          // Destructor
          ~DialogController();
          // Is handle to modal dialog
          bool IsModal() { return _type == HandleType::Modal ? true : false; }
          // Set type of dialog handle
          void Type(HandleType value) { _type = value; }
          virtual bool OnInit() { return false; }
        private:
          HandleType _type; // Type of dialog handle
        };
      }
    }
  }

