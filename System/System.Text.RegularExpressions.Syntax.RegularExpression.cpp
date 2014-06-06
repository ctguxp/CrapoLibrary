#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.RegularExpression.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        RegularExpression::RegularExpression()
          {
          }
        RegularExpression::~RegularExpression()
          {
          }
        /*void RegularExpression::Compile(ICompiler* cmp, bool reverse)
          {
          int min;
          int max;
          GetWidth(out min, out max);
          cmp->EmitInfo(this.group_count, min, max);
          AnchorInfo anchorInfo = this.GetAnchorInfo(reverse);
          LinkRef linkRef = cmp.NewLink();
          cmp->EmitAnchor(reverse, anchorInfo.Offset, linkRef);
          if (anchorInfo.IsPosition)
            {
            cmp->EmitPosition(anchorInfo.Position);
            }
          else
            {
            if(anchorInfo.IsSubstring)
              {
              cmp->EmitString(anchorInfo.Substring, anchorInfo.IgnoreCase, reverse);
              }
            }
          cmp->EmitTrue();
          cmp->ResolveLink(linkRef);
          Group::Compile(cmp, reverse);
          cmp->EmitTrue();
          }*/
        }
      }
    }
  }
