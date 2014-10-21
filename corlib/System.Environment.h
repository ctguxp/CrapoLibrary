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
#pragma warning(disable:4251)
    static GCOperatingSystem _os;
#pragma warning(default:4251)
    static String _nl;
    public:
      Environment();
      ~Environment();

      // Fields
      static String CurrentDirectory();
      static void CurrentDirectory(String&);

      static String MachineName();
      static String NewLine();
      static String SystemDirectory();
      static ulong TickCount();
      static String UserName();
      static String GetOSVersionString();
      static OperatingSystem* OSVersion();
      static PlatformID Platform();

      static bool IsRunningOnWindows();
    private:
      static String GetNewLine();
    };
  }
