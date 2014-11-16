#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"
#include "System.Text.RegularExpressions.Interval.h"
#include "System.Text.RegularExpressions.Category.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class CharacterClass : public Expression
          {
          private:
            static Interval       upper_case_characters;
		        int                   _distance_between_upper_and_lower_case;
		        bool                  _negate;
            bool                  _ignore;
		        Collections::BitArray _pos_cats;
            Collections::BitArray _neg_cats;
		        IntervalCollection    _intervals;
          public:
            CharacterClass(bool negate = false, bool ignore = true);
            CharacterClass(Category cat, bool negate);
            ~CharacterClass();
            void AddCharacter(wchar_t c);
            void AddRange(wchar_t lo, wchar_t hi);
            void AddCategory(Category cat, bool negate);
            virtual void Compile(ICompiler* /*cmp*/, bool /*reverse*/) override;
            virtual void GetWidth(int& /*min*/, int& /*max*/) override;
            virtual bool IsComplex() override;
          private:
            static double GetIntervalCost(Interval* i); 
          };
        }
      }
    }
  }
