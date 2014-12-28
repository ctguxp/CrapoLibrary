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
          :_group_count(0)
          {
          }
        RegularExpression::~RegularExpression()
          {
          }
        int RegularExpression::GroupCount()
          {
          return _group_count;
          }
        void RegularExpression::GroupCount(int value)
          {
          _group_count = value;
          }
        void RegularExpression::Compile(ICompiler* cmp, bool reverse)
          {
          int min;
          int max;
          GetWidth(min, max);
          cmp->EmitInfo(_group_count, min, max);
          GCAnchorInfo anchorInfo = GetAnchorInfo(reverse);
          LinkRef* linkRef = cmp->NewLink();
          cmp->EmitAnchor(reverse, anchorInfo->Offset(), linkRef);
          if(anchorInfo->IsPosition())
            {
            cmp->EmitPosition(anchorInfo->Position());
            }
          else
            {
            if(anchorInfo->IsSubstring())
              {
              cmp->EmitString(anchorInfo->Substring(), anchorInfo->IgnoreCase(), reverse);
              }
            }
          cmp->EmitTrue();
          cmp->ResolveLink(linkRef);
          Group::Compile(cmp, reverse);
          cmp->EmitTrue();
          }
        }
      }
    }
  }
