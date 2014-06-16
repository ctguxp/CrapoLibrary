#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"
#include "System.Text.RegularExpressions.Syntax.ExpressionCollection.h"

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
            virtual bool IsComplex() override;
          protected:  
            CompositeExpression();
            ExpressionCollection& Expressions() { return _expressions; }
            virtual void GetWidth(int& /*min*/, int& /*max*/) override {}
            void GetWidth(int32& /*min*/, int32& /*max*/, int32 /*count*/);
          private:
            ExpressionCollection _expressions;
          };
        }
      }
    }
  }

