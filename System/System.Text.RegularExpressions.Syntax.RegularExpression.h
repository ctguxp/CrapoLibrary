#pragma once
#include "System.Text.RegularExpressions.ICompiler.h"
#include "System.Text.RegularExpressions.Syntax.Group.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class RegularExpression : public Group
          {
          public:
            RegularExpression();
            ~RegularExpression();
            int GroupCount();
            void GroupCount(int value);
            virtual void Compile(ICompiler* cmp, bool reverse) override;
          private:
            int _group_count;
          };
        }
      }
    }
  }

