#pragma once
#include "AutoPtr.h"
#include "System.PlatformID.h"
#include "System.Version.h"

namespace System
  {
  class OperatingSystem
    {
    public:
      OperatingSystem(PlatformID /*platform*/, Version /*version*/);
      ~OperatingSystem();
    private:
      PlatformID _platform;
      Version    _version;
    };
  typedef AutoPtr<OperatingSystem> GCOperatingSystem;
  }
