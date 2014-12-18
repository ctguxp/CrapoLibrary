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
          :_min(min),
          _max(max),
          _lazy(lazy)
          {
          //GCObject null;
          //_expressions.Add(null);
          }

        Repetition::~Repetition()
          {
          }

        Syntax::Expression* Repetition::Expression()
          {
          return (Syntax::Expression*)_expressions[0].Get();
          }

        void Repetition::Expression(GCExpression& value)
          {
          _expressions.Add(value);
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
        }
      }
    }
  }
