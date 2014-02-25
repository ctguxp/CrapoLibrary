#pragma once
#include "System.IO.IOException.h"

namespace System
  {
  namespace IO
    {
    class FileNotFoundException : public IOException
      {
      public:
        FileNotFoundException();
        FileNotFoundException(cstring);
        FileNotFoundException(cstring, Exception&);
        FileNotFoundException(const FileNotFoundException&);
        ~FileNotFoundException();
        FileNotFoundException& operator = (const FileNotFoundException&);
      };
    }
  }

