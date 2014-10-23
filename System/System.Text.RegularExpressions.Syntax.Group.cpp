#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Group.h"

using namespace Global;

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        Group::Group()
          {
          }
        Group::~Group()
          {
          }
        void Group::AppendExpression(GCExpression& e)
          {
          Expressions().Add(e);
          }
        void Group::Compile(ICompiler* cmp, bool reverse)
          {
          int32 count = Expressions().Count();
          for(int32 i = 0; i < count; ++ i)
            {
            Expression* e = nullptr;
            if(reverse)
              e = static_cast<Expression*>(Expressions()[count - i - 1].Get());
            else
              e = static_cast<Expression*>(Expressions()[i].Get());

            e->Compile(cmp, reverse);
            }
          }
        void Group::GetWidth(int& min, int& max)
          {
          min = 0;
          max = 0;

          for(int32 i = 0; i < Expressions().Count(); ++i)
            {
            Expression* expression = static_cast<Expression*>(Expressions()[i].Get());
            int a, b;
            expression->GetWidth(a, b);
            min += a;
            if(max == Int32::MaxValue || b == Int32::MaxValue)
              max = Int32::MaxValue;
            else
              max += b;
            }
          }
        }
      }
    }
  }
