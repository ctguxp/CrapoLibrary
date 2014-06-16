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
        class ExpressionCollection : public Collections::CollectionBase
          {
          public:
            ExpressionCollection();
            ~ExpressionCollection();
            void Add(Expression* e);
            virtual sizet Add(Object* value) override; 
          };
        }
      }
    }
  }
