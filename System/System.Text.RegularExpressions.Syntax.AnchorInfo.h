#pragma
#include "System.Text.RegularExpressions.Position.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class Expression;
        class AnchorInfo
          {
          public:
            AnchorInfo(Expression& expr, int width);
            AnchorInfo(const AnchorInfo& anchor);
            ~AnchorInfo();
            AnchorInfo& operator=(const AnchorInfo& anchor);
            Expression& Expression();
            bool IsPosition();
            int Offset();
            int Width();
            int Length();
            RegularExpressions::Position Position();
            bool IgnoreCase();
            bool IsSubstring();
            String& Substring();
          private:
            Syntax::Expression*           _expr;
            RegularExpressions::Position  _pos;
            int                           _offset;
            String*                       _str;
            int                           _width;
            bool                          _ignore;
          };
        }
      }
    }
  }
