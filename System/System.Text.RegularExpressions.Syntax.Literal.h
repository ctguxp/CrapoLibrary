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
            System::String _str;
            bool   _ignore;
          public:
            Literal(System::String str, bool ignore);
            ~Literal();
            System::String String();
            void String(System::String value);
            bool IgnoreCase();
            void IgnoreCase(bool value);
            virtual void Compile(ICompiler* /*cmp*/, bool /*reverse*/) override;
		        virtual void GetWidth(int32& /*min*/, int32& /*max*/) override;
            virtual bool IsComplex() override;
            static void CompileLiteral(System::String str, ICompiler* cmp, bool ignore, bool reverse);
          };
        typedef SharedPtr<Literal> GCLiteral;
        }
      }
    }
  }
