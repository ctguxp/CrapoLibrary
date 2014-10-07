#pragma once
#include "System.Text.RegularExpressions.Syntax.Expression.h"

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
            //static Interval upper_case_characters = new Interval ((char)65, (char)90);
		        int  _distance_between_upper_and_lower_case;
		        bool _negate;
            bool _ignore;
		        //BitArray pos_cats, neg_cats;
		        //IntervalCollection intervals;
          public:
            CharacterClass(bool negate = false, bool ignore = true);
            ~CharacterClass();
            void AddCharacter(wchar_t c); 
            virtual void Compile(ICompiler* /*cmp*/, bool /*reverse*/) override;
            virtual void GetWidth(int& /*min*/, int& /*max*/) override;
            virtual bool IsComplex() override;
          };
        }
      }
    }
  }
