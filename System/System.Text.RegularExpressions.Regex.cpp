#include "pch.h"
#include "System.Text.RegularExpressions.Regex.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      Regex::Regex()
        :_pattern()
        ,_roptions(RegexOptions::None)
        ,_machineFactory(nullptr)
        {
        }

      Regex::Regex(String pattern, RegexOptions options)
        :_pattern(pattern)
        ,_roptions(options)
        ,_machineFactory(nullptr)
        {
        ValidateOptions(options);
        }

      Regex::~Regex()
        {
        }

      void Regex::ValidateOptions(RegexOptions options)
        {
        intptr allopts =
          (intptr)RegexOptions::None |
          (intptr)RegexOptions::IgnoreCase |
          (intptr)RegexOptions::Multiline |
          (intptr)RegexOptions::ExplicitCapture |
          (intptr)RegexOptions::Singleline |
          (intptr)RegexOptions::IgnorePatternWhitespace |
          (intptr)RegexOptions::RightToLeft |
          (intptr)RegexOptions::ECMAScript |
          (intptr)RegexOptions::CultureInvariant;

        intptr ecmaopts =
          (intptr)RegexOptions::IgnoreCase |
          (intptr)RegexOptions::Multiline |
          (intptr)RegexOptions::ECMAScript;

        if(((intptr)options & ~allopts) != 0)
          throw ArgumentOutOfRangeException(L"options");
        if(((intptr)options & (intptr)RegexOptions::ECMAScript) != 0 && ((intptr)options & ~ecmaopts) != 0)
          throw ArgumentOutOfRangeException(L"options");
        }

      void Regex::Init()
        {
        
        }

      }
    }
  }
