#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.PositionAssertion.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        PositionAssertion::PositionAssertion(Position pos)
          :_pos(pos)
          {
          }
        PositionAssertion::~PositionAssertion()
          {
          }
        void PositionAssertion::Compile(ICompiler* cmp, bool)
          {
          cmp->EmitPosition(_pos);
          }
        void PositionAssertion::GetWidth(int& min, int& max) 
          {
          min = max = 0;
          }
        bool PositionAssertion::IsComplex() 
          {
          return false;
          }
        }
      }
    }
  }
