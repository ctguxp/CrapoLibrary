#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.CompositeExpression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        CompositeExpression::CompositeExpression()
          :_expressions()
          {
          }
        CompositeExpression::~CompositeExpression()
          {
          }
        bool CompositeExpression::IsComplex ()
          {
          for(sizet i = 0; i < Expressions().Count(); ++i)
            {
            Expression* expression = static_cast<Expression*>(&Expressions()[i]);
            if(expression->IsComplex())
              return true;
            }
          return GetFixedWidth() <= 0;
          }
        void CompositeExpression::GetWidth(int32& min, int32& max, int32 count)
          {
          min = 2147483647;
          max = 0;
          bool flag = true;
          for(int i = 0; i < count; i++)
            {
            Expression* expression = static_cast<Expression*>(&Expressions()[i]);
            if(expression != nullptr)
              {
              flag = false;
              int num;
              int num2;
              expression->GetWidth(num, num2);
              if(num < min)
                {
                min = num;
                }
              if(num2 > max)
                {
                max = num2;
                }
              }
            }
          if (flag)
            {
            min = (max = 0);
            }
          }
        }
      }
    }
  }
