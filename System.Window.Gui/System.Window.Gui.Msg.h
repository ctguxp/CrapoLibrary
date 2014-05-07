// Perch Lake Computer System

#pragma once

namespace System
  {
  namespace Window
    {
    namespace Gui
      { 
      class Msg
        {
        public:
          // Default constructor
          Msg();
          // Destructor
          ~Msg();
          // Dispatch message
          void Dispatch();
          // Get message
          BOOL Get(HWND hwnd = NULL, uint32 msgFilterMin = 0, uint32 msgFilterMax = 0);
          // Is dialog
          BOOL IsDialog(HWND hwnd);
          // Translate message
          void Translate();
          // Property wparam
          WPARAM Wparam() const { return _wparam; }
        private:
          HWND        _hwnd;    // A handle to the window whose window procedure receives the message
          uint32      _message; // The message identifier
          WPARAM      _wparam;  // Additional information about the message
          LPARAM      _lparam;  // Additional information about the message
          ulong       _time;    // The time at which the message was posted
          POINT       _pt;      // The cursor position, in screen coordinates, when the message was posted
        };
      }
    }
  }