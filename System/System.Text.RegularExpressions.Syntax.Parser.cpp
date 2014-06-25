#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Parser.h"
#include "System.Text.RegularExpressions.Syntax.Assertion.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        Parser::Parser()
          :_pattern()
          ,_ptr(0)
          ,_numGroups(0)
          ,_caps()
          ,_refs()
          {
          }
        Parser::~Parser()
          {
          }
        RegularExpression* Parser::ParseRegularExpression(String pattern, RegexOptions options)
          {
          _pattern = pattern;
          _ptr = 0;

          _caps.Clear();
          _refs.Clear();
          _numGroups = 0;

          try 
            {
            RegularExpression* re = new RegularExpression();
            ParseGroup((Group*)re, options, nullptr);
            //ResolveReferences();

            //re->GroupCount(num_groups);

            return re;
            }
          catch(Exception&/*IndexOutOfRangeException*/)
            {
            //throw NewParseException ("Unexpected end of pattern.");
            throw SystemException(L"Unexpected end of pattern.");
            }
          }
        int Parser::GetMapping(Collections::Hashtable* mapping)
          {
          sizet end = _caps.Count();
          mapping->Add(new String(L"0"), new Int32(0));
          for(sizet i = 0; i < end; i++) 
            {
            //CapturingGroup group = (CapturingGroup)caps[i];
            //string name = group.Name != null ? group.Name : group.Index.ToString ();
            //if (mapping.Contains (name)) 
              ///{
              //if ((int) mapping [name] != group.Index)
                //throw new SystemException ("invalid state");
              //;
              //
            //mapping.Add (name, group.Index);
            }
          throw NotImplementedException();
          //return gap;
          }
        void Parser::ParseGroup(Group* /*group*/, RegexOptions /*options*/, Assertion* /*assertion*/) 
          {
          //bool is_top_level = group is RegularExpression;

          //Alternation* alternation = nullptr;
          //string lit;

          //Group* current = new Group );
          //Expression* expr = nullptr;
          //bool closed = false;

          //          while(true) 
          //            {
          //            ConsumeWhitespace(IsIgnorePatternWhitespace (options));
          //            if(_ptr >= _pattern.Length())
          //              break;
          //
          //            // (1) Parse for Expressions
          //
          //            wchar_t ch = pattern[_ptr++];
          //
          //            switch(ch) 
          //              {
          //              case L'^': 
          //                {
          //                Position pos =
          //                  IsMultiline (options) ? Position.StartOfLine : Position.Start;
          //                expr = new PositionAssertion (pos);
          //                break;
          //                }
          //
          //              case L'$': 
          //                {
          //                Position pos =
          //                  IsMultiline (options) ? Position.EndOfLine : Position.End;
          //                expr = new PositionAssertion (pos);
          //                break;
          //                }
          //
          //              case L'.': 
          //                {
          //                Category cat =
          //                  IsSingleline (options) ? Category.AnySingleline : Category.Any;
          //                expr = new CharacterClass (cat, false);
          //                break;
          //                }
          //
          //              case L'\\': 
          //                {
          //                int c = ParseEscape (false);
          //                if (c >= 0)
          //                  ch = (char)c;
          //                else 
          //                  {
          //                  expr = ParseSpecial (options);
          //
          //                  if (expr == null)
          //                    ch = pattern[ptr ++];		// default escape
          //                  }
          //                break;
          //                }
          //
          //              case L'[': 
          //                {
          //                expr = ParseCharacterClass (options);
          //                break;
          //                }
          //
          //              case L'(': 
          //                {
          //                bool ignore = IsIgnoreCase (options);
          //                expr = ParseGroupingConstruct (ref options);
          //                if (expr == null) {
          //                  if (lit != null && IsIgnoreCase (options) != ignore) {
          //                    current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
          //                    lit = null;
          //                    }
          //
          //                  continue;
          //                  }
          //                break;
          //                }
          //
          //              case L')': 
          //                {
          //                closed = true;
          //                goto EndOfGroup;
          //                }
          //
          //              case L'|': 
          //                {
          //                if (lit != null) 
          //                  {
          //                  current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
          //                  lit = null;
          //                  }
          //
          //                if (assertion != null) 
          //                  {
          //                  if (assertion.TrueExpression == null)
          //                    assertion.TrueExpression = current;
          //                  else if (assertion.FalseExpression == null)
          //                    assertion.FalseExpression = current;
          //                  else
          //                    throw NewParseException ("Too many | in (?()|).");
          //                  }
          //                else 
          //                  {
          //                  if (alternation == null)
          //                    alternation = new Alternation ();
          //
          //                  alternation.AddAlternative (current);
          //                  }
          //
          //                current = new Group ();
          //                continue;
          //                }
          //
          //              case L'*': case L'+': case L'?': 
          //                {
          //                throw NewParseException ("Bad quantifier.");
          //                }
          //
          //              default: 
          //                break;		// literal character
          //              }
          //
          //            ConsumeWhitespace (IsIgnorePatternWhitespace (options));
          //
          //            // (2) Check for Repetitions
          //
          //            if(_ptr < pattern.Length())
          //              {
          //              wchar_t k = pattern[ptr];
          //              int min = 0, max = 0;
          //              bool lazy = false;
          //              bool haveRep = false;
          //
          //
          //              if (k == L'?' || k == L'*' || k == L'+') 
          //                {
          //                ++ ptr;
          //                haveRep = true;
          //
          //                switch (k) 
          //{
          //                  case L'?': min = 0; max = 1; break;
          //                  case L'*': min = 0; max = 0x7fffffff; break;
          //                  case L'+': min = 1; max = 0x7fffffff; break;
          //                  }
          //                } 
          //              else if (k == L'{' && ptr + 1 < pattern.Length) 
          //                {
          //                int saved_ptr = _ptr;
          //                ++_ptr;
          //                haveRep = ParseRepetitionBounds (out min, out max, options);
          //                if (!haveRep)
          //                  _ptr = saved_ptr;
          //                }
          //
          //              if (haveRep) 
          //                {
          //                ConsumeWhitespace (IsIgnorePatternWhitespace (options));
          //                if (ptr < pattern.Length && pattern[ptr] == L'?')
          //                  {
          //                  ++_ptr;
          //                  lazy = true;
          //                  }
          //
          //                //It doesn't make sense to assert a given position more than once.
          //                bool ignore_repetition = false;
          //                if (expr is PositionAssertion) 
          //                  {
          //                  ignore_repetition = min > 0 && !lazy;
          //                  max = 1;
          //                  }
          //
          //                if (!ignore_repetition) 
          //                  {
          //                  Repetition repetition = new Repetition (min, max, lazy);
          //
          //                  if (expr == null)
          //                    repetition.Expression = new Literal (ch.ToString (), IsIgnoreCase (options));
          //                  else
          //                    repetition.Expression = expr;
          //
          //                  expr = repetition;
          //                  }
          //                }
          //              }
          //
          //            // (3) Append Expression and/or Literal
          //
          //            if (expr == null) 
          //              {
          //              if (lit == null)
          //                lit = L"";
          //              lit += ch;
          //              }
          //            else 
          //              {
          //              if (lit != null) 
          //                {
          //                current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
          //                lit = null;
          //                }
          //
          //              current.AppendExpression (expr);
          //              expr = null;
          //              }
          //
          //            if (is_top_level && ptr >= pattern.Length)
          //              goto EndOfGroup;
          //            }
          //
          //EndOfGroup:
          //          if (is_top_level && closed)
          //            throw NewParseException ("Too many )'s.");
          //          if (!is_top_level && !closed)
          //            throw NewParseException ("Not enough )'s.");
          //
          //
          //          // clean up literals and alternations
          //
          //          if (lit != null)
          //            current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
          //
          //          if (assertion != null) 
          //            {
          //            if (assertion.TrueExpression == null)
          //              assertion.TrueExpression = current;
          //            else
          //              assertion.FalseExpression = current;
          //
          //            group.AppendExpression (assertion);
          //            }
          //          else if (alternation != null) 
          //            {
          //            alternation.AddAlternative (current);
          //            group.AppendExpression (alternation);
          //            }
          //          else
          //            group.AppendExpression (current);
          //          }
          }
        }
      }
    }
  }
