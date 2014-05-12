#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"
#include "System.Text.RegularExpressions.Cache.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Regex
        {
        static FactoryCache cache;
        public:
          Regex(String, RegexOptions);
          ~Regex();
        protected:
          Regex();
          static void ValidateOptions(RegexOptions);  
        private:
          void Init();
          void InitNewRegex();
          static IMachineFactory* CreateMachineFactory(String pattern, RegexOptions options); 
        protected:
          int                       _groupCount;
          int                       _gap;
          String                    _pattern;
          RegexOptions              _roptions;
          IMachineFactory*          _machineFactory;
          Collections::IDictionary* _mapping;
          StringArray               _groupNames;
        };
      }
    }
  }
