// Perch Lake Computer System
// Golden on 6/13/2014

#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Expression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        Expression::Expression()
          {
          }
        Expression::~Expression()
          {
          }
        int Expression::GetFixedWidth()
          {
          int32 num;
          int32 num2;
          GetWidth(num, num2);
          if(num == num2)
            {
            return num;
            }
          return -1;
          }
        GCAnchorInfo Expression::GetAnchorInfo(bool)
		      {
			    return GCAnchorInfo(new AnchorInfo(*this, GetFixedWidth()));
		      }
        }
      }
    }
  }
