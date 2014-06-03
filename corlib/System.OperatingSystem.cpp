#include "pch.h"
#include "System.OperatingSystem.h"

namespace System
  {
  OperatingSystem::OperatingSystem(PlatformID platform, Version version)
    :_platform(platform)
    ,_version(version)
    {
    }
  OperatingSystem::~OperatingSystem()
    {
    }
  }
