// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.Array.hpp"

namespace System
  {
  class CRAPOCOREDLL_API Environment
    {
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

      static bool IsRunningOnWindows();
    };
  }
