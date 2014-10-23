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
        class Literal : public Expression
          {
          private:
            String _str;
            bool   _ignore;
          public:
            Literal(String str, bool ignore);
            ~Literal();
            virtual void Compile(ICompiler* /*cmp*/, bool /*reverse*/) override;
		        virtual void GetWidth(int& /*min*/, int& /*max*/) override;
            virtual bool IsComplex() override;
          };
        }
      }
    }
  }
