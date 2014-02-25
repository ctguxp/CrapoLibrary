#pragma once
#include "System.IO.IOException.h"

namespace System
  {
  namespace IO
    {
    class DirectoryNotFoundException : public IOException
      {
      public:
        DirectoryNotFoundException();
        DirectoryNotFoundException(cstring);
        DirectoryNotFoundException(cstring, Exception&);
        DirectoryNotFoundException(const IOException&);
        ~DirectoryNotFoundException();
        DirectoryNotFoundException& operator = (const DirectoryNotFoundException&);
      };
    }
  }
