#pragma once
#include "System.String.h"

namespace System
  {
  class Version
    {
    enum
      {
      UNDEFINED = -1
      };
    public:
      Version(int major = 0, int minor = 0);
      Version(int major, int minor, int build);
      Version(int major, int minor, int build, int revision);
      Version(String ver);
      Version(const Version& ver);
      ~Version();
      Version& operator=(const Version& version);
    private:
      void CheckedSet(sizet defined, int major, int minor, int build, int revision);
    private:
      int _major;
      int _minor;
      int _build;
      int _revision;
    };
  }
