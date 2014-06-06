#pragma once
#include "System.Text.RegualarExpressions.ICompiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Expression
          {
          public:
            ~Expression();
            virtual void Compile(ICompiler* cmp, bool reverse) = 0;
		        virtual void GetWidth(int& min, int& max) = 0;
            int GetFixedWidth();
            // TODO virtual AnchorInfo GetAnchorInfo(bool reverse);
            virtual bool IsComplex() = 0;
          protected:
            Expression();
          };
        }
      }
    }
  }
