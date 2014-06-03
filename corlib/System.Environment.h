// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Array.hpp"
#include "System.OperatingSystem.h"
#include "System.PlatformID.h"

namespace System
  {
  class CRAPOCOREDLL_API Environment
    {
    static GCOperatingSystem _os;
    public:
      Environment();
      ~Environment();

      // Fields
      static String CurrentDirectory();
      static void CurrentDirectory(String&);

      static String MachineName();
      static String SystemDirectory();
      static ulong TickCount();
      static String UserName();
      static String GetOSVersionString();
      static OperatingSystem* OSVersion();
      static PlatformID Platform();

      static bool IsRunningOnWindows();
    };
  }
