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
        Literal::Literal(System::String str, bool ignore)
          :_str(str)
          ,_ignore(ignore)
          {
          }
        Literal::~Literal()
          {
          }
        System::String Literal::String()
          { 
          return _str; 
          }
        void Literal::String(System::String value)
          { 
          _str = value; 
          }
        bool Literal::IgnoreCase()
          { 
          return _ignore; 
          }
        void Literal::IgnoreCase(bool value)
          { 
          _ignore = value; 
          }
        void Literal::Compile(ICompiler* cmp, bool reverse)
          {
          CompileLiteral(_str, cmp, _ignore, reverse);
          }
        void Literal::GetWidth(int32& min, int32& max)
          {
          min = max = _str.Length();
          }
        bool Literal::IsComplex()
          {
          return false;
          }
        void Literal::CompileLiteral(System::String str, ICompiler* cmp, bool ignore, bool reverse)
          {
          if(str.Length() == 0)
            return;

          if(str.Length() == 1)
            cmp->EmitCharacter(str[0], false, ignore, reverse);
          else
            cmp->EmitString(str, ignore, reverse);
          }
        }
      }
    }
  }
