// Perch Lake Computer System

#pragma once

namespace System
  {
  namespace Window
    {
    namespace Gui
      {
      class CommonControl
        {
        public:
          // Default constructor
          CommonControl();
          // Destructor
          ~CommonControl();
          // Ensures that the common control DLL get loaded (Put in WinMain)
          void Initialize();
        private:
          ulong _size;  // Size of the structure
          ulong _icc;   // The set of bit flags that indicate which common control classes will be loaded from the DLL
        };
      }
    }
  }

