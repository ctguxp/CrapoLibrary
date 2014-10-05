#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.Cache.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Match;
      class IMachine;
      class Regex
        {
        static FactoryCache cache;
        public:
          Regex(String, RegexOptions);
          ~Regex();
          RegularExpressions::Match* Match(String input, int startat);
          int32 Gap() { return _gap; }
        protected:
          Regex();
          static void ValidateOptions(RegexOptions);  
        private:
          IMachine* CreateMachine(); 
          void Init();
          void InitNewRegex();
          static IMachineFactory* CreateMachineFactory(String pattern, RegexOptions options);
          static StringArray GetGroupNamesArray(int groupCount, Collections::IDictionary* mapping);
        protected:
          int                       _groupCount;
          int32                     _gap;
          String                    _pattern;
          RegexOptions              _roptions;
          IMachineFactory*          _machineFactory;
          Collections::IDictionary* _mapping;
          StringArray               _groupNames;
        };
      }
    }
  }
