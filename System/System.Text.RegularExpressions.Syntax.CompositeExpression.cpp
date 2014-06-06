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
          {
          }
        CompositeExpression::~CompositeExpression()
          {
          }
        void CompositeExpression::GetWidth(int32& min, int32& max, int32 count)
          {
          throw NotImplementedException();
          /*min = 2147483647;
          max = 0;
          bool flag = true;
          for(int i = 0; i < count; i++)
            {
            Expression expression = this.Expressions[i];
            if(expression != null)
              {
              flag = false;
              int num;
              int num2;
              expression.GetWidth(out num, out num2);
              if (num < min)
                {
                min = num;
                }
              if (num2 > max)
                {
                max = num2;
                }
              }
            }
          if (flag)
            {
            min = (max = 0);
            }*/
          }
        }
      }
    }
  }
