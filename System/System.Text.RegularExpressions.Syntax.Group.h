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
        class Group : public CompositeExpression
          {
          public:
            Group();
            ~Group();
            void AppendExpression(Expression* e);
            virtual void Compile(ICompiler* cmp, bool reverse) override;
            virtual void GetWidth(int& min, int& max) override;
          };
        }
      }
    }
  }
