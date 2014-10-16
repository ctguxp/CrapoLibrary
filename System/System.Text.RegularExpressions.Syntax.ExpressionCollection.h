#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"

using namespace Global;

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
            void Add(SharedPtr<Expression>& e);
            virtual sizet Add(GCObject& value) override; 
          };
        }
      }
    }
  }
