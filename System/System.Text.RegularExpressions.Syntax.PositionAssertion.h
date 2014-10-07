#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"
#include "System.Text.RegularExpressions.Position.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
       namespace Syntax
        {
        class PositionAssertion : public Expression
          {
          private:
            Position _pos;
          public:
            PositionAssertion(Position pos = Position::Any);
            ~PositionAssertion();
            virtual void Compile(ICompiler* /*cmp*/, bool /*reverse*/) override;
            virtual void GetWidth(int& min, int& max) override;
            virtual bool IsComplex() override;
          };
        }
      }
    }
  }
