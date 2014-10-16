#pragma once
#include "System.Text.RegularExpressions.Syntax.CompositeExpression.h"

using namespace Global;

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
            void AppendExpression(SharedPtr<Expression> e);
            virtual void Compile(ICompiler* cmp, bool reverse) override;
            virtual void GetWidth(int& min, int& max) override;
          };
        }
      }
    }
  }
