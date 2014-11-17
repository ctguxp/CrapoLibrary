#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.Cache.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"
#include "System.Text.RegularExpressions.Match.h"

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
          Regex(String&, RegexOptions);
          ~Regex();
          GCMatch Match(cstring input, int startat);
          GCMatch Match(String& input, int startat);
          int32 Gap() { return _gap; }
          bool IsMatch(String& input);
          bool IsMatch(String& input, int32 startat);
          static bool IsMatch(String& input, String& pattern);
          static bool IsMatch(String& input, String& pattern, RegexOptions options);
          bool RightToLeft();
        protected:
          Regex();
          static void ValidateOptions(RegexOptions);  
        private:
          GCIMachine CreateMachine(); 
          void Init();
          void InitNewRegex();
          int DefaultStartat(String& input);
          static IMachineFactory* CreateMachineFactory(String& pattern, RegexOptions options);
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
