#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.CharacterClass.h"
#include "System.Text.RegularExpressions.Interval.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {

        Interval CharacterClass::upper_case_characters((wchar_t)65, (wchar_t)90);

        CharacterClass::CharacterClass(bool negate, bool ignore)
          :_distance_between_upper_and_lower_case(32)
          ,_negate(negate)
          ,_ignore(ignore)
          ,_pos_cats((int32)Category::LastValue)
          ,_neg_cats((int32)Category::LastValue)
          {
          }
        CharacterClass::CharacterClass(Category cat, bool negate)
          :_negate(false)
          ,_ignore(false)
          {
          AddCategory(cat, negate);
          }
        CharacterClass::~CharacterClass()
          {
          }
        void CharacterClass::AddCharacter(wchar_t c)
          {
          // TODO: this is certainly not the most efficient way of doing things
          // TODO: but at least it produces correct results.
          AddRange(c, c);
          }
        void CharacterClass::AddRange(wchar_t lo, wchar_t hi) 
          {
          Interval* new_interval = new Interval(lo, hi);

          //// ignore case is on. we must make sure our interval does not
          //// use upper case. if it does, we must normalize the upper case
          //// characters into lower case.
          if(_ignore) 
            {
            if(upper_case_characters.Intersects(*new_interval))
              {
              Interval* partial_new_interval = nullptr;

              if(new_interval->Low() < upper_case_characters.Low())
                {
                partial_new_interval = new Interval(upper_case_characters.Low() + _distance_between_upper_and_lower_case,
                  new_interval->High() +  _distance_between_upper_and_lower_case);
                new_interval->High(upper_case_characters.Low() - 1);
                }
              else 
                {
                partial_new_interval = new Interval (new_interval->Low() + _distance_between_upper_and_lower_case,
                  upper_case_characters.High() + _distance_between_upper_and_lower_case);
                new_interval->Low(upper_case_characters.High() + 1);
                }
              _intervals.Add(partial_new_interval);
              }
            else if(upper_case_characters.Contains(*new_interval)) 
              {
              new_interval->High(new_interval->High() + _distance_between_upper_and_lower_case);
              new_interval->Low(new_interval->Low() + _distance_between_upper_and_lower_case);
              }
            }
          _intervals.Add(new_interval);
          }

        void CharacterClass::AddCategory(Category cat, bool negate)
          {
          int32 n = (int32)cat;

          if(negate)
            {
            _neg_cats.Set(n, true);
            } 
          else {
            _pos_cats.Set(n, true);
            }
          }

        void CharacterClass::Compile(ICompiler* /*cmp*/, bool /*reverse*/) 
          {
          // create the meta-collection
          //IntervalCollection* meta = _intervals.GetMetaCollection(new IntervalCollection.CostDelegate(GetIntervalCost));

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
