#pragma once
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API NotImplementedException : public SystemException
    {
    public:
      NotImplementedException(cstring message = nullptr);
      NotImplementedException(cstring, Exception&);
      NotImplementedException(const NotImplementedException&);
      ~NotImplementedException();
      NotImplementedException& operator = (const NotImplementedException&);
    };
  }

