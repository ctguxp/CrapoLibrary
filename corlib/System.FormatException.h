#pragma once
#include "System.Exception.h"

namespace System
  {
  class FormatException : public SystemException
    {
    public:
      FormatException(cstring message = nullptr);
      ~FormatException();
    };
  }
