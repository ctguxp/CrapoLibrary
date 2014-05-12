#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.Syntax.RegularExpression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Parser
          {
          public:
            Parser();
            ~Parser();
            RegularExpression* ParseRegularExpression(String pattern, RegexOptions options);
          private:
            // TODO void ParseGroup(Group group, RegexOptions options, Assertion assertion);
          private:
            String                 _pattern;
            int                    _ptr;
            int                    _numGroups;
            Collections::ArrayList _caps;
            Collections::Hashtable _refs;
          };
        }
      }
    }
  }