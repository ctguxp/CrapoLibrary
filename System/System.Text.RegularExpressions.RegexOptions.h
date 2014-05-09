#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      enum class RegexOptions
        {
        None				            = 0x000,
        IgnoreCase			        = 0x001,
        Multiline			          = 0x002,
        ExplicitCapture			    = 0x004,
        Singleline			        = 0x010,
        IgnorePatternWhitespace = 0x020,
        RightToLeft			        = 0x040,
        ECMAScript			        = 0x100,
        CultureInvariant		    = 0x200 
        };
      }
    }
  }
