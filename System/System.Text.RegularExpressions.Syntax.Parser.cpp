#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.Parser.h"
#include "System.Text.RegularExpressions.Syntax.Assertion.h"
#include "System.Text.RegularExpressions.Syntax.Alternation.h"
#include "System.Text.RegularExpressions.Syntax.PositionAssertion.h"
#include "System.Text.RegularExpressions.Syntax.CharacterClass.h"
#include "System.Text.RegularExpressions.Category.h"
#include "System.Text.RegularExpressions.Syntax.Literal.h"
#include "System.Text.RegularExpressions.Syntax.CapturingGroup.h"

using namespace Global;

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
          ,_gap(0)
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
            SharedPtr<Assertion> assert;
            ParseGroup((Group*)re, options, assert);
            ResolveReferences();

            re->GroupCount(_numGroups);

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
          //throw NotImplementedException();
          return _gap;
          }
        void Parser::ParseGroup(Group* group, RegexOptions options, SharedPtr<Assertion>& assertion) 
          {
          RegularExpression g;
          bool is_top_level = Object::IsInstance((*group), g);
          if(!is_top_level)
            return;

          SharedPtr<Alternation> alternation;
          String lit;

          SharedPtr<Group> current(new Group());
          SharedPtr<Expression> expr;
          bool closed = false;

          for(;;) 
            {
            ConsumeWhitespace(IsIgnorePatternWhitespace(options));
            if(_ptr >= _pattern.Length())
              break;

            // (1) Parse for Expressions

            wchar_t ch = _pattern[_ptr++];

            switch(ch) 
              {
              case L'^': 
                {
                Position pos = IsMultiline(options) ? Position::StartOfLine : Position::Start;
                expr.Reset(new PositionAssertion(pos));
                break;
                }

              case L'$': 
                {
                Position pos = IsMultiline (options) ? Position::EndOfLine : Position::End;
                expr.Reset(new PositionAssertion(pos));
                break;
                }

              case L'.': 
                {
                Category cat = IsSingleline(options) ? Category::AnySingleline : Category::Any;
                expr.Reset(new CharacterClass(cat, false));
                break;
                }

              case L'\\': 
                {
                //int c = ParseEscape(false);
                //if (c >= 0)
                //  ch = (wchar_t)c;
                //else 
                //  {
                //  expr = ParseSpecial(options);

                if(expr.Get() == nullptr)
                  ch = _pattern[_ptr ++];		// default escape
                //}
                break;
                }

              case L'[': 
                {
                expr = ParseCharacterClass(options);
                break;
                }

              case L'(': 
                {
                //bool ignore = IsIgnoreCase(options);
                /*expr = ParseGroupingConstruct(ref options);
                if (expr == nullptr)
                {
                if(lit != nullptr && IsIgnoreCase (options) != ignore)
                {
                current->AppendExpression(new Literal(lit, IsIgnoreCase (options)));
                lit = String::Empty();
                }

                continue;
                }*/
                break;
                }

              case L')': 
                {
                closed = true;
                goto EndOfGroup;
                }

              case L'|': 
                {
                if (lit.Length() != 0) 
                  {
                  //current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
                  lit = String::Empty();
                  }

                if(assertion.Get() != nullptr) 
                  {
                  /*if (assertion.TrueExpression == null)
                  assertion.TrueExpression = current;
                  else if (assertion.FalseExpression == null)
                  assertion.FalseExpression = current;
                  else
                  throw NewParseException(L"Too many | in (?()|).");*/
                  }
                else 
                  {
                  if(alternation.Get() == nullptr){}
                  //alternation = new Alternation();

                  //alternation->AddAlternative(current);
                  }

                current.Reset(new Group());
                continue;
                }

              case L'*': case L'+': case L'?': 
                {
                //throw NewParseException(L"Bad quantifier.");
                }

              default: 
                break;		// literal character
              }

            ConsumeWhitespace(IsIgnorePatternWhitespace(options));

            // (2) Check for Repetitions

            if(_ptr < _pattern.Length())
              {
              wchar_t k = _pattern[_ptr];
              int min = 0, max = 0;
              bool lazy = false;
              bool haveRep = false;


              if(k == L'?' || k == L'*' || k == L'+') 
                {
                ++ _ptr;
                haveRep = true;

                switch(k) 
                  {
                  case L'?': min = 0; max = 1; break;
                  case L'*': min = 0; max = 0x7fffffff; break;
                  case L'+': min = 1; max = 0x7fffffff; break;
                  }
                } 
              else if(k == L'{' && _ptr + 1 < _pattern.Length()) 
                {
                int saved_ptr = _ptr;
                ++_ptr;
                //haveRep = ParseRepetitionBounds (out min, out max, options);
                //if (!haveRep)
                _ptr = saved_ptr;
                }

              if(haveRep) 
                {
                ConsumeWhitespace(IsIgnorePatternWhitespace(options));
                if(_ptr < _pattern.Length() && _pattern[_ptr] == L'?')
                  {
                  ++_ptr;
                  lazy = true;
                  }

                // It doesn't make sense to assert a given position more than once.
                bool ignore_repetition = false;
                PositionAssertion pos;
                if(Object::IsInstance(*expr, pos)) 
                  {
                  ignore_repetition = min > 0 && !lazy;
                  max = 1;
                  }

                if(!ignore_repetition) 
                  {
                  //    Repetition repetition = new Repetition (min, max, lazy);

                  //    if (expr == null)
                  //      repetition.Expression = new Literal (ch.ToString (), IsIgnoreCase (options));
                  //    else
                  //      repetition.Expression = expr;

                  //    expr = repetition;
                  }
                }
              }

            // (3) Append Expression and/or Literal

            if(expr.Get() == nullptr) 
              {
              if(lit.Length() == 0)
                lit = L"";
              lit += ch;
              }
            else 
              {
              if (lit.Length() != 0) 
                {
                //current.AppendExpression (new Literal (lit, IsIgnoreCase (options)));
                lit = String::Empty();
                }

              current->AppendExpression(expr);
              expr.Reset();
              }

            if (is_top_level && _ptr >= _pattern.Length())
              goto EndOfGroup;
            }

EndOfGroup:
          if (is_top_level && closed)
            //throw NewParseException(L"Too many )'s.");
              throw NotImplementedException();
          if (!is_top_level && !closed)
            //throw NewParseException ("Not enough )'s.");
              throw NotImplementedException();


          // clean up literals and alternations

          if(lit.Length() != 0)
            {
            GCExpression l(new Literal(lit, IsIgnoreCase(options)));
            current->AppendExpression(l);
            }

          if (assertion.Get() != nullptr) 
            {
            /*if(assertion->TrueExpression == null)
            assertion->TrueExpression = current;
            else
            assertion->FalseExpression = current;*/

            GCExpression a = assertion;
            group->AppendExpression(a);
            }
          else if(alternation.Get() != nullptr) 
            {
            //alternation->AddAlternative (current);
            GCExpression a = alternation;
            group->AppendExpression(a);
            }
          else
            {
            GCExpression e = current;
            group->AppendExpression(e);
            }
          }
        SharedPtr<Expression> Parser::ParseCharacterClass(RegexOptions options) 
          {
          bool negate = false;
          if(_pattern[_ptr] == L'^') 
            {
            negate = true;
            ++_ptr;
            }

          bool ecma = IsECMAScript(options);
          if(ecma == true){} // delete me
          SharedPtr<CharacterClass> cls(new CharacterClass(negate, IsIgnoreCase(options)));

          if(_pattern[_ptr] == L']')
            {
            cls->AddCharacter(L']');
            ++_ptr;
            }

          int c = -1;
          int last = -1;
          bool range = false;
          bool closed = false;
          while(_ptr < _pattern.Length())
            {
            c = _pattern[_ptr ++];

            if (c == L']') 
              {
              closed = true;
              break;
              }

            if(c == L'-' && last >= 0 && !range)
              {
              range = true;
              continue;
              }

            if (c == L'\\') 
              {
              //c = ParseEscape(true);
              if (c >= 0)
                goto char_recognized;

              // didn't recognize escape
              c = _pattern[_ptr ++];
              switch (c) 
                {
                case L'b':
                  c = L'\b';
                  goto char_recognized;

                case L'd': case L'D':
                  cls->AddCategory(ecma ? Category::EcmaDigit : Category::Digit, c == L'D');
                  break;

                case L'w': case L'W':
                  cls->AddCategory(ecma ? Category::EcmaWord : Category::Word, c == L'W');
                  break;

                case 's': case 'S':
                  cls->AddCategory(ecma ? Category::EcmaWhiteSpace : Category::WhiteSpace, c == 'S');
                  break;

                case 'p': case 'P':
                  //cls->AddCategory(ParseUnicodeCategory (), c == :'P');	// ignore ecma
                  break;

                default:		// add escaped character
                  goto char_recognized;
                }

              // if the pattern looks like [a-\s] ...
              if (range){}
              //throw NewParseException ("character range cannot have category \\" + c);

              last = -1;
              continue;
              }

char_recognized:
            if(range) 
              {
              // if 'range' is true, we know that 'last >= 0'
              if(c < last)
                //throw NewParseException ("[" + last + "-" + c + "] range in reverse order.");
                throw SystemException(L"Range in reverse order");
              cls->AddRange((wchar_t)last, (wchar_t)c);
              last = -1;
              range = false;
              continue;
              }

            cls->AddCharacter((wchar_t)c);
            last = c;
            }

          if(!closed){}
          //throw NewParseException ("Unterminated [] set.");

          if(range)
            cls->AddCharacter(L'-');

          return cls;
          }
        void Parser::ResolveReferences()
          {
          using namespace Collections;
          int32 gid = 1;
          Hashtable dict;
          ArrayList explicit_numeric_groups;

          //// number unnamed groups
          IEnumerator* caps = _caps.GetEnumerator();
          while(caps->MoveNext())
            {
            GCObject group = caps->Current();
            CapturingGroup& cg = static_cast<CapturingGroup&>((*group)); 
            Int32 g(gid);
            String gstr = g.ToString();
            dict.Add(&gstr, group.Get());
            cg.Index(gid++);
            ++_numGroups;
            }
          caps->Reset();

          //// number named groups
          while(caps->MoveNext())
            {
            GCObject group = caps->Current();
            CapturingGroup& cg = static_cast<CapturingGroup&>((*group));
            if(dict.Contains(cg.Name()))
              {
              }
            }
          delete caps;
          caps = nullptr;
          //foreach (CapturingGroup group in caps) 
          //  {
          //  if (group.Name == null)
          //    continue;

          //  if (dict.Contains (group.Name)) 
          //    {
          //    CapturingGroup prev = (CapturingGroup) dict [group.Name];
          //    group.Index = prev.Index;

          //    if (group.Index == gid)
          //      gid ++;
          //    else if (group.Index > gid)
          //      explicit_numeric_groups.Add (group);
          //    continue;
          //    }

          //  if (Char.IsDigit (group.Name [0])) 
          //    {
          //    int ptr = 0;
          //    int group_gid = ParseDecimal (group.Name, ref ptr);
          //    if (ptr == group.Name.Length) 
          //      {
          //      group.Index = group_gid;
          //      dict.Add (group.Name, group);
          //      ++ num_groups;

          //      if (group_gid == gid) 
          //        {
          //        gid ++;
          //        } 
          //      else 
          //        {
          //        // all numbers before 'gid' are already in the dictionary.  So, we know group_gid > gid
          //        if (explicit_numeric_groups == null)
          //          explicit_numeric_groups = new ArrayList (4);
          //        explicit_numeric_groups.Add (group);
          //        }

          //      continue;
          //      }
          //    }

          //  string gid_s = gid.ToString ();
          //  while (dict.Contains (gid_s))
          //    gid_s = (++gid).ToString ();

          //  dict.Add (gid_s, group);
          //  dict.Add (group.Name, group);
          //  group.Index = gid ++;
          //  ++ num_groups;
          //  }

          _gap = gid; // == 1 + num_groups, if explicit_numeric_groups == null

          //if (explicit_numeric_groups != null)
          //  HandleExplicitNumericGroups (explicit_numeric_groups);

          //// resolve references

          //foreach (Expression expr in refs.Keys) 
          //  {
          //  string name = (string) refs [expr];
          //  if (!dict.Contains (name)) 
          //    {
          //    if (expr is CaptureAssertion && !Char.IsDigit (name [0]))
          //      continue;
          //    BackslashNumber bn = expr as BackslashNumber;
          //    if (bn != null && bn.ResolveReference (name, dict))
          //      continue;
          //    throw NewParseException ("Reference to undefined group " +
          //      (Char.IsDigit (name[0]) ? "number " : "name ") +
          //      name);
          //    }

          //  CapturingGroup group = (CapturingGroup)dict[name];
          //  if (expr is Reference)
          //    ((Reference)expr).CapturingGroup = group;
          //  else if (expr is CaptureAssertion)
          //    ((CaptureAssertion)expr).CapturingGroup = group;
          //  else if (expr is BalancingGroup)
          //    ((BalancingGroup)expr).Balance = group;
          //  }
          }

        void Parser::ConsumeWhitespace(bool ignore)
          {
          while(_ptr < _pattern.Length()) 
            {
            if(_pattern[_ptr] == '(') 
              {
              if(_ptr + 3 >= _pattern.Length())
                return;

              if(_pattern[_ptr + 1] != L'?' || _pattern[_ptr + 2] != L'#')
                return;

              _ptr += 3;
              while(_ptr < _pattern.Length() && _pattern[_ptr ++] != L')')
                /* ignore */ ;
              }
            else if(ignore && _pattern[_ptr] == L'#') 
              {
              while(_ptr < _pattern.Length() && _pattern[_ptr ++] != L'\n')
                /* ignore */ ;
              }
            else if (ignore && Char::IsWhiteSpace(_pattern[_ptr])) 
              {
              while(_ptr < _pattern.Length() && Char::IsWhiteSpace(_pattern[_ptr]))
                ++_ptr;
              }
            else
              return;
            }
          }
        bool Parser::IsIgnorePatternWhitespace(RegexOptions options)
          {
          return((intptr)options & (intptr)RegexOptions::IgnorePatternWhitespace) != 0;
          }
        bool Parser::IsECMAScript(RegexOptions options) 
          {
          return((intptr)options & (intptr)RegexOptions::ECMAScript) != 0;
          }
        bool Parser::IsIgnoreCase(RegexOptions options) 
          {
          return((intptr)options & (intptr)RegexOptions::IgnoreCase) != 0;
          }
        bool Parser::IsMultiline(RegexOptions options) 
          {
          return((intptr)options & (intptr)RegexOptions::Multiline) != 0;
          }
        bool Parser::IsSingleline(RegexOptions options) 
          {
          return((intptr)options & (intptr)RegexOptions::Singleline) != 0;
          }
        }
      }
    }
  }
