#pragma once
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API NotSupportedException : public SystemException
    {
    public:
      NotSupportedException(cstring message = nullptr);
      NotSupportedException(cstring, Exception&);
      NotSupportedException(const NotSupportedException&);
      ~NotSupportedException();
      NotSupportedException& operator = (const NotSupportedException&);
    };
  }

