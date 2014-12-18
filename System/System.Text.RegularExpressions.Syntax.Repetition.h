#pragma once
#include "System.Text.RegularExpressions.Syntax.CompositeExpression.h"
#include "System.Text.RegularExpressions.Syntax.Expression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Repetition : public CompositeExpression
          {
          public:
            Repetition(int min, int max, bool lazy);
            ~Repetition();
            virtual void Compile(ICompiler* cmp, bool reverse) override;
            Expression* Expression();
            void Expression(GCExpression& value);
          private:
            int _min;
            int _max;
            bool _lazy;
          };
        typedef SharedPtr<Repetition> GCRepetition;
        }
      }
    }
  }
