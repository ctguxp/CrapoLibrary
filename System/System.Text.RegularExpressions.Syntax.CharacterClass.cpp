#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.CharacterClass.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        CharacterClass::CharacterClass(bool negate, bool ignore)
          :_distance_between_upper_and_lower_case(32)
          ,_negate(negate)
          ,_ignore(ignore)
          {
          }
        CharacterClass::~CharacterClass()
          {
          }
        void CharacterClass::AddCharacter(wchar_t /*c*/)
          {
          // TODO: this is certainly not the most efficient way of doing things
          // TODO: but at least it produces correct results.
          // TODO: AddRange (c, c);
          throw NotImplementedException();
          }
        void CharacterClass::Compile(ICompiler* /*cmp*/, bool /*reverse*/) 
          {
          // create the meta-collection
          // TODO IntervalCollection meta = intervals.GetMetaCollection (new IntervalCollection.CostDelegate (GetIntervalCost));

          throw NotImplementedException();

          // count ops
          /*int count = meta.Count;
          for (int i = 0; i < pos_cats.Length; ++ i) {
          if (pos_cats[i] || neg_cats [i])
          ++ count;
          }*/

          /* if (count == 0)
          return;*/

          // emit in op for |meta| > 1
          /* LinkRef tail = cmp->NewLink();
          if (count > 1)
          cmp->EmitIn(tail);*/

          // emit character/range/sets from meta-collection
          // we emit these first so that any 'ignore' flags will be noticed by the evaluator
          //foreach (Interval a in meta) 
          //  {
          //  if (a.IsDiscontiguous) 
          //    {			// Set
          //    BitArray bits = new BitArray (a.Size);
          //    foreach (Interval b in intervals) 
          //      {
          //      if (a.Contains (b)) 
          //        {
          //        for (int i = b.low; i <= b.high; ++ i)
          //          bits[i - a.low] = true;
          //        }
          //      }

          //    cmp.EmitSet ((char)a.low, bits, negate, ignore, reverse);
          //    }
          //  else if (a.IsSingleton)				// Character
          //    cmp.EmitCharacter ((char)a.low, negate, ignore, reverse);
          //  else						// Range
          //    cmp.EmitRange ((char)a.low, (char)a.high, negate, ignore, reverse);
          //  }

          //// emit categories
          //for (int i = 0; i < pos_cats.Length; ++ i) 
          //  {
          //  if (pos_cats[i]) {
          //    if (neg_cats [i])
          //      cmp.EmitCategory (Category.AnySingleline, negate, reverse);
          //    else
          //      cmp.EmitCategory ((Category)i, negate, reverse);
          //    } 
          //  else if (neg_cats[i]) {
          //    cmp.EmitNotCategory ((Category)i, negate, reverse);
          //    }
          //  }

          //// finish up
          //if (count > 1) 
          //  {
          //  if (negate)
          //    cmp.EmitTrue ();
          //  else
          //    cmp.EmitFalse ();

          //  cmp.ResolveLink (tail);
          //  }
          }
        void CharacterClass::GetWidth(int& min, int& max) 
          {
          min = max = 1;
          }
        bool CharacterClass::IsComplex() 
          {
          return false;
          }
        }
      }
    }
  }
