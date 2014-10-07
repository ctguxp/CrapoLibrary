#pragma once
#include "System.Text.RegularExpressions.Syntax.CompositeExpression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Alternation : public CompositeExpression
          {
          public:
            Alternation();
            ~Alternation();
          };
        }
      }
    }
  }
