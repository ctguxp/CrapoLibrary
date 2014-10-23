// Perch Lake Computer System
// Golden on 6/13/2014

#pragma once
#include "System.Object.h"
#include "System.Text.RegularExpressions.ICompiler.h"
#include "System.Text.RegularExpressions.Syntax.AnchorInfo.h"
#include "Global.Memory.h"

using namespace Global;

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Expression : public Object
          {
          public:
            virtual ~Expression();
            virtual void Compile(ICompiler* cmp, bool reverse) = 0;
		        virtual void GetWidth(int& min, int& max) = 0;
            int GetFixedWidth();
            AnchorInfo GetAnchorInfo(bool reverse);
            virtual bool IsComplex() = 0;
          protected:
            Expression();
          };
        typedef SharedPtr<Expression> GCExpression;
        }
      }
    }
  }
