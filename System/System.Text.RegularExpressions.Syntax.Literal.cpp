#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Literal.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        Literal::Literal(String str, bool ignore)
          :_str(str)
          ,_ignore(ignore)
          {
          }
        Literal::~Literal()
          {
          }
        void Literal::Compile(ICompiler* /*cmp*/, bool /*reverse*/)
          {
          }
		    void Literal::GetWidth(int& /*min*/, int& /*max*/)
          {
          }
        bool Literal::IsComplex()
          {
          return false;
          }
        }
      }
    }
  }
