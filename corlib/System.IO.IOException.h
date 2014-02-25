#pragma once
#include "System.Exception.h"

namespace System
  {
  namespace IO
    {
    class IOException : public SystemException
      {
      public:
        IOException();
        IOException(cstring);
        IOException(cstring, Exception&);
        IOException(cstring, int);
        IOException(const IOException&);
        ~IOException();
        IOException& operator = (const IOException&);
      };
    }
  }

