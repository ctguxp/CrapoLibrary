#pragma once
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API OutOfMemoryException : public SystemException
    {
    public:
      OutOfMemoryException(cstring = nullptr);
      OutOfMemoryException(cstring, Exception&);
      OutOfMemoryException(const OutOfMemoryException&);
      ~OutOfMemoryException();
      OutOfMemoryException& operator = (const OutOfMemoryException&);
    };
  }
