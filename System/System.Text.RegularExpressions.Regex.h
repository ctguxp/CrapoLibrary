#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Regex
        {
        public:
          Regex(String, RegexOptions);
          ~Regex();
        protected:
          Regex();
          static void ValidateOptions(RegexOptions);  
        private:
          void Init();
        protected:
          String           _pattern;
          RegexOptions     _roptions;
          IMachineFactory* _machineFactory;
        };
      }
    }
  }
