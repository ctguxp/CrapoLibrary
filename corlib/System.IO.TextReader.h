#pragma once
#include "System.String.h"

namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API TextReader
      {
      public:
        virtual ~TextReader();
        virtual String ReadToEnd();
        virtual int Read();
      protected:
        TextReader();
      };
    }
  }

