#pragma once
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API FormatException : public SystemException
    {
    public:
      FormatException(cstring message = nullptr);
      ~FormatException();
    };
  }
