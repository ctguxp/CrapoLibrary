#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Repetition.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        Repetition::Repetition(int min, int max, bool lazy)
          :_min(min)
          ,_max(max)
          ,_lazy(lazy)
          {
          //GCObject null;
          //_expressions.Add(null);
          }

        Repetition::~Repetition()
          {
          }

        int Repetition::Maximum()
          {
          return _max; 
          }
        void Repetition::Maximum(int value)
          { 
          _max = value; 
          }

        int Repetition::Minimum()
          { 
          return _min; 
          }
        void Repetition::Minimum(int value)
          { 
          _min = value; 
          }

        Syntax::Expression* Repetition::Expression()
          {
          return (Syntax::Expression*)_expressions[0].Get();
          }

        void Repetition::Expression(GCExpression& value)
          {
          _expressions.Add(value);
          }

        void Repetition::GetWidth(int& min, int& max) 
          {
          Expression()->GetWidth(min, max);
          min = min * _min;
          if(max == Int32::MaxValue || _max == 0xffff)
            max = Int32::MaxValue;
          else
            max = max * _max;
          }

        void Repetition::Compile(ICompiler* cmp, bool reverse) 
          {
          if(Expression()->IsComplex ()) 
            {
            LinkRef* until = cmp->NewLink ();

            cmp->EmitRepeat(_min, _max, _lazy, until);
            Expression()->Compile(cmp, reverse);
            cmp->EmitUntil(until);
            }
          else 
            {
            LinkRef* tail = cmp->NewLink ();

            cmp->EmitFastRepeat(_min, _max, _lazy, tail);
            Expression()->Compile(cmp, reverse);
            cmp->EmitTrue ();
            cmp->ResolveLink(tail);
            }
          }

        GCAnchorInfo Repetition::GetAnchorInfo(bool reverse)
          {
          int width = GetFixedWidth ();
          if(Minimum() == 0)
            return GCAnchorInfo(new AnchorInfo(*this, width));

          GCAnchorInfo info = Expression()->GetAnchorInfo(reverse);
          if(info->IsPosition())
            return GCAnchorInfo(new AnchorInfo(*this, info->Offset(), width, info->Position()));

          if(info->IsSubstring()) 
            {
            if(info->IsComplete()) 
              {
              // Minimum > 0
              String str = info->Substring();
              StringBuilder sb(str);
              for(int i = 1; i < Minimum(); ++ i)
                sb.Append(str);

              return GCAnchorInfo(new AnchorInfo(*this, 0, width, sb.ToString(), info->IgnoreCase()));
              }

            return GCAnchorInfo(new AnchorInfo(*this, info->Offset(), width, info->Substring(), info->IgnoreCase()));
            }

          return GCAnchorInfo(new AnchorInfo(*this, width));
          }
        }
      }
    }
  }
