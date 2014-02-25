#pragma once
#include "System.IO.FileAttributes.h"

namespace System
  {
  namespace IO
    {
    struct CrapoIOStat
      {
      FileAttributes Attributes;
      long Length;
      long CreationTime;
      long LastAccessTime;
      long LastWriteTime;
      };
    }
  }