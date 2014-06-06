#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class CompositeExpression : public Expression
          {
          public:
            ~CompositeExpression();
          protected:  
            CompositeExpression();
            void GetWidth(int32& min, int32& max, int32 count);
          };
        }
      }
    }
  }

