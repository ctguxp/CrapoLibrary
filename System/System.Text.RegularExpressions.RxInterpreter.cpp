#include "pch.h"
#include "System.Text.RegularExpressions.RxInterpreter.h"
#include "System.Text.RegularExpressions.RxOp.h"
#include "System.Text.RegularExpressions.Match.h"
#include "System.Text.RegularExpressions.Group.h"
#include "System.Text.RegularExpressions.Category.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      RxInterpreter::IntStack::IntStack()
        :_values(0)
        ,_count(0)
        {
        }

      int RxInterpreter::IntStack::Pop()
        {
        return _values [--_count];
        }
      void RxInterpreter::IntStack::Push(int value)
        {
        if(_values.IsNull())
          {
          _values.Length(8);
          } 
        else if(_count == _values.Length())
          {
          sizet new_size = _values.Length();
          new_size += new_size >> 1;
          _values.Length(new_size);
          }
        _values[_count++] = value;
        }

      sizet RxInterpreter::IntStack::Count()
        {
        return _count;
        }
      void RxInterpreter::IntStack::Count(sizet value)
        {
        if(value > _count)
          throw SystemException(L"can only truncate the stack");
        _count = value;
        }

      RxInterpreter::RepeatContext::RepeatContext(RxInterpreter::RepeatContext* previous, int min, int max, bool lazy, int expr_pc)
        :_start(-1)
        ,_min(min)
        ,_max(max)
        ,_lazy(lazy)
        ,_expr_pc(expr_pc)
        ,_previous(previous)
        ,_count(0)
        {
        }

      RxInterpreter::RepeatContext::~RepeatContext()
        {
        }

      RxInterpreter::RxInterpreter(ByteArray _program, EvalDelegate eval_del)
        :_program(_program)
        ,_eval_del(eval_del)
        ,_group_count(1 + (_program[1] | ((int)_program[2] << 8)))
        ,_groups(_group_count)
        {
        ResetGroups();
        }
      RxInterpreter::~RxInterpreter()
        {
        }
      Match* RxInterpreter::Scan(Regex* regex, String text, int start, int end)
        {
        _str = text;
        _string_start = start;
        _string_end = end;
        int res = 0;

        bool match;
        if(_eval_del != nullptr)
          {
          match = _eval_del(this, start, res);
          } 
        else 
          {
          match = EvalByteCode(11, start, res);
          }
        _marks[_groups[0]].End = res;
        if(match) 
          {
          return GenerateMatch(regex);
          }
        return Match::Empty();
        }

      void RxInterpreter::GetGroupInfo(int gid, int& first_mark_index, int& n_caps)
        {
        first_mark_index = -1;
        n_caps = 0;
        for(int m = _groups[gid]; m >= 0; m = _marks[m].Previous)
          {
          if(!_marks[m].IsDefined())
            continue;
          if(first_mark_index < 0)
            first_mark_index = m;
          ++n_caps;
          }
        }

      void RxInterpreter::PopulateGroup(Group* g, int first_mark_index, int n_caps)
        {
        int i = 1;
        for (int m = _marks[first_mark_index].Previous; m >= 0; m = _marks[m].Previous)
          {
          if(!_marks[m].IsDefined())
            continue;
          Capture* cap = new Capture(_str, _marks[m].Index(), _marks[m].Length());
          g->Captures().SetValue(cap, n_caps - 1 - i);
          ++i;
          }
        }

      Match* RxInterpreter::GenerateMatch(Regex* regex)
        {
        int n_caps = 0;
        int first_mark_index = 0;
        Group* g;
        GetGroupInfo(0, first_mark_index, n_caps);

        // Avoid fully populating the Match instance if not needed
        if(!_needs_groups_or_captures)
          return new Match(regex, this, _str, _string_end
          ,_marks[first_mark_index].Index(), _marks[first_mark_index].Length());

        Match* retval = new Match(regex, this, _str, _string_end, (int)_groups.Length() 
          ,_marks[first_mark_index].Index(), _marks[first_mark_index].Length(), n_caps);

        PopulateGroup(retval, first_mark_index, n_caps);

        for(sizet gid = 1; gid < _groups.Length(); ++ gid)
          {
          GetGroupInfo((int)gid, first_mark_index, n_caps);
          if (first_mark_index < 0)
            {
            g = Group::Fail();
            } 
          else 
            {
            g = new Group(_str, _marks [first_mark_index].Index(), _marks [first_mark_index].Length(), n_caps);

            PopulateGroup(g, first_mark_index, n_caps);
            }
          retval->Groups().SetValue(g, (int)gid);
          }
        return retval;
        }

      void RxInterpreter::ResetGroups()
        {
        sizet n = _groups.Length();
        if(_marks.IsNull())
          _marks.Length(n);

        for(sizet i = 0; i < n; ++ i) 
          {
          _groups[i] = (int)i;

          _marks[i].Start = -1;
          _marks[i].End = -1;
          _marks[i].Previous = -1;
          }
        _mark_start = 0;
        _mark_end = n;
        }

      int RxInterpreter::GetLastDefined(int gid) 
        {
        int m = _groups[gid];
        while (m >= 0 && !_marks[m].IsDefined())
          m = _marks[m].Previous;

        return m;
        }

      // capture management
      void RxInterpreter::Open(int gid, int ptr) 
        {
        sizet m = _groups[gid];
        if (m < _mark_start || _marks[m].IsDefined()) 
          {
          m = CreateMark((int)m);
          _groups[gid] = (int)m;
          }

        _marks[m].Start = ptr;
        }

      int RxInterpreter::CreateMark(int previous) 
        {
        if(_mark_end == _marks.Length()) 
          {
          _marks.Length(_marks.Length() * 2);
          }

        int m = (int)_mark_end ++;
        _marks[m].Start = _marks[m].End = -1;
        _marks [m].Previous = previous;

        return m;
        }

      void RxInterpreter::Close(int gid, int ptr)
        {
        _marks[_groups[gid]].End = ptr;
        }

      bool RxInterpreter::Balance(int gid, int balance_gid, bool capture, int ptr) 
        {
        int b = _groups[balance_gid];

        if(b == -1 || _marks[b].Index() < 0)
          {
          //Group not previously matched
          return false;
          }
        //Debug.Assert(marks [b].IsDefined, "Regex", "Balancng group not closed");
        if (gid > 0 && capture)
          { 
          Open(gid, _marks[b].Index() + _marks[b].Length());
          Close(gid, ptr);
          }

        _groups[balance_gid] = _marks[b].Previous;

        return true;
        }

      bool RxInterpreter::IsWordChar(wchar_t c)
        {
        return Char::IsLetterOrDigit(c) || Char::GetUnicodeCategory(c) == Globalization::UnicodeCategory::ConnectorPunctuation;
        }

      int RxInterpreter::Checkpoint()
        {
        _mark_start = _mark_end;
        return (int)_mark_start;
        }

      void RxInterpreter::Backtrack(int cp) 
        {
        for(sizet i = 0; i < _groups.Length(); ++ i) 
          {
          int m = _groups[i];
          while(cp <= m)
            m = _marks[m].Previous;
          _groups[i] = m;
          }
        }

      int RxInterpreter::ReadInt(ByteArray& code, int pc)
        {
        int val = code [pc];
        val |= (int)code[pc + 1] << 8;
        val |= (int)code[pc + 2] << 16;
        val |= (int)code[pc + 3] << 24;
        return val;
        }

      bool RxInterpreter::EvalByteCode(int pc, int strpos, int& strpos_result)
        {
        using namespace Globalization;
        // luckily the IL engine can deal with char_group_end at compile time
        // this code offset needs to be checked only in opcodes that handle
        // a single wchar_t and that are included in a TestCharGroup expression:
        // the engine is supposed to jump to this offset as soons as the
        // first opcode in the expression matches
        // The code pattern becomes:
        // on successfull match: check if char_group_end is nonzero and jump to
        // test_char_group_passed after adjusting strpos
        // on failure: try the next expression by simply advancing pc
        int char_group_end = 0;
        int length, start, end;
        for( ; ; ) 
          {
          //if (trace_rx) 
          //  {
          //  Console.WriteLine ("evaluating: {0} at pc: {1}, strpos: {2}, cge: {3}", (RxOp)_program [pc], pc, strpos, char_group_end);
          //  //Console.WriteLine ("deep: " + (deep == null ? 0 : deep.GetHashCode ()) + " repeat: " + (this.repeat == null ? 0 : this.repeat.GetHashCode ()));
          //  }
          switch ((RxOp)_program[pc]) 
            {
            case RxOp::True: // 2
              if (char_group_end != 0) 
                {
                pc = char_group_end;
                char_group_end = 0;
                continue;
                }
              strpos_result = strpos;
              return true;
            case RxOp::False: //2
              return false;
            case RxOp::AnyPosition:
              pc++;
              continue;
            case RxOp::StartOfString:
              if (strpos != 0)
                return false;
              pc++;
              continue;
            case RxOp::StartOfLine:
              if (strpos == 0 || _str[strpos - 1] == L'\n') 
                {
                pc++;
                continue;
                }
              return false;
            case RxOp::StartOfScan:
              if (strpos != _string_start)
                return false;
              pc++;
              continue;
            case RxOp::End:
              if (strpos == _string_end || (strpos == _string_end - 1 && _str [strpos] == L'\n')) 
                {
                pc++;
                continue;
                }
              return false;
            case RxOp::EndOfString:
              if (strpos != _string_end)
                return false;
              pc++;
              continue;
            case RxOp::EndOfLine:
              if (strpos == _string_end || _str [strpos] == L'\n') 
                {
                pc++;
                continue;
                }
              return false;
            case RxOp::WordBoundary:
              if (_string_end == 0)
                return false;
              if (strpos == 0) 
                {
                if (IsWordChar (_str [strpos])) 
                  {
                  pc++;
                  continue;
                  }
                } 
              else if (strpos == _string_end) 
                {
                if (IsWordChar (_str [strpos - 1])) 
                  {
                  pc++;
                  continue;
                  }
                } 
              else 
                {
                if (IsWordChar (_str [strpos]) != IsWordChar (_str [strpos - 1])) 
                  {
                  pc++;
                  continue;
                  }
                }
              return false;
            case RxOp::NoWordBoundary:
              if (_string_end == 0)
                return false;
              if (strpos == 0) 
                {
                if (!IsWordChar (_str [strpos]))
                  {
                  pc++;
                  continue;
                  }
                } 
              else if (strpos == _string_end) 
                {
                if (!IsWordChar (_str [strpos - 1])) 
                  {
                  pc++;
                  continue;
                  }
                } 
              else
                {
                if (IsWordChar (_str [strpos]) == IsWordChar (_str [strpos - 1])) 
                  {
                  pc++;
                  continue;
                  }
                }
              return false;
            case RxOp::Anchor: //150
              {
              int skip = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              int anch_offset = _program [pc + 3] | ((int)_program [pc + 4] << 8);

              /*
              * In the general case, we have to evaluate the bytecode
              * starting at pc + skip, however the optimizer emits some
              * special cases, whose bytecode begins at pc + 5.
              */
              int anch_pc = pc + 5;
              RxOp anch_op = (RxOp)(_program[anch_pc] & 0x00ff);

              bool spec_anch = false;

              // FIXME: Add more special cases from interpreter.cs
              if (anch_op == RxOp::String || anch_op == RxOp::StringIgnoreCase) 
                {
                if (pc + skip == anch_pc + 2 + _program [anch_pc + 1] + 1)
                  {
                  // Anchor
                  //	String
                  //	True
                  spec_anch = true;
                  /*if (trace_rx)
                  Console.WriteLine ("  string anchor at {0}, offset {1}", anch_pc, anch_offset);*/
                  }
                }

              pc += skip;

              if ((RxOp)_program[pc] == RxOp::StartOfString)
                {
                if (strpos == 0) {
                  int res = strpos;
                  if (_groups.Length() > 1) 
                    {
                    ResetGroups();
                    _marks [_groups [0]].Start = strpos;
                    }
                  if (EvalByteCode (pc + 1, strpos, res)) 
                    {
                    _marks [_groups [0]].Start = strpos;
                    if (_groups.Length() > 1)
                      _marks[_groups [0]].End = res;
                    strpos_result = res;
                    return true;
                    }
                  }
                return false;
                }

              // it's important to test also the end of the string
              // position for things like: "" =~ /$/
              end = _string_end + 1;
              while (strpos < end) 
                {
                if (spec_anch) {
                  if (anch_op == RxOp::String || anch_op == RxOp::StringIgnoreCase) 
                    {
                    /* 
                    * This means the match must contain a given
                    * string at a constant position, so we can skip 
                    * forward until the string matches. This is a win if
                    * the rest of the regex 
                    * has a complex positive lookbehind for example.
                    */
                    int tmp_res = strpos;
                    if (!EvalByteCode (anch_pc, strpos + anch_offset, tmp_res)) 
                      {
                      strpos ++;
                      continue;
                      }
                    }
                  }
                int res = strpos;
                if (_groups.Length() > 1) 
                  {
                  ResetGroups ();
                  _marks [_groups [0]].Start = strpos;
                  }
                if (EvalByteCode (pc, strpos, res)) 
                  {
                  //							match_start = strpos;
                  _marks [_groups [0]].Start = strpos;
                  if (_groups.Length() > 1)
                    _marks [_groups [0]].End = res;
                  strpos_result = res;
                  return true;
                  }
                strpos++;
                }
              return false;
              }
            case RxOp::AnchorReverse:
              length = _program [pc + 3] | ((int)_program [pc + 4] << 8);
              pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
              // it's important to test also the end of the string
              // position for things like: "" =~ /$/
              end = 0;
              while (strpos >= 0) 
                {
                int res = strpos;
                if (_groups.Length() > 1) 
                  {
                  ResetGroups ();
                  _marks [_groups [0]].Start = strpos;
                  }
                if (EvalByteCode (pc, strpos, res)) 
                  {
                  //							match_start = strpos;
                  _marks [_groups [0]].Start = strpos;
                  if (_groups.Length() > 1)
                    _marks [_groups [0]].End = res;
                  strpos_result = res;
                  return true;
                  }
                strpos--;
                }
              return false;
            case RxOp::Reference:
              length = GetLastDefined(_program [pc + 1] | ((int)_program [pc + 2] << 8));
              if (length < 0)
                return false;
              start = _marks[length].Index();
              length = _marks[length].Length();
              if (strpos + length > _string_end)
                return false;
              for (end = start + length; start < end; ++start) 
                {
                if (_str [strpos] != _str [start])
                  return false;
                strpos++;
                }
              pc += 3;
              continue;
            case RxOp::ReferenceIgnoreCase:
              length = GetLastDefined (_program [pc + 1] | ((int)_program [pc + 2] << 8));
              if (length < 0)
                return false;
              start = _marks[length].Index();
              length = _marks[length].Length();
              if (strpos + length > _string_end)
                return false;
              for (end = start + length; start < end; ++start) 
                {
                if (_str [strpos] != _str [start] && Char::ToLower (_str [strpos]) != Char::ToLower (_str [start]))
                  return false;
                strpos++;
                }
              pc += 3;
              continue;
            case RxOp::ReferenceReverse: 
              {
              length = GetLastDefined (_program [pc + 1] | ((int)_program [pc + 2] << 8));
              if (length < 0)
                return false;
              start = _marks[length].Index();
              length = _marks[length].Length();
              if (strpos - length < 0)
                return false;
              int p = strpos - length;
              for (end = start + length; start < end; ++start, ++p) 
                {
                if (_str [p] != _str [start])
                  return false;
                }
              strpos -= length;
              pc += 3;
              continue;
              }
            case RxOp::IfDefined:
              if (GetLastDefined (_program [pc + 3] | ((int)_program [pc + 4] << 8)) >= 0)
                pc += 5;
              else
                pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
              continue;
            case RxOp::SubExpression: {
              int res = 0;
              if (EvalByteCode (pc + 3, strpos, res)) 
                {
                pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
                strpos = res;
                continue;
                }
              return false;
              }
            case RxOp::Test: 
              {
              int res = 0;
              // FIXME: checkpoint
              if (EvalByteCode (pc + 5, strpos, res))
                {
                pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
                } 
              else
                {
                pc += _program [pc + 3] | ((int)_program [pc + 4] << 8);
                }
              continue;
              }
            case RxOp::OpenGroup:
              Open(_program [pc + 1] | ((int)_program [pc + 2] << 8), strpos);
              pc += 3;
              continue;
            case RxOp::CloseGroup:
              Close(_program [pc + 1] | ((int)_program [pc + 2] << 8), strpos);
              pc += 3;
              continue;
            case RxOp::BalanceStart: 
              {
              int res = 0;

              if (!EvalByteCode (pc + 8, strpos, res))
                goto Fail;

              int gid = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              int balance_gid = _program [pc + 3] | ((int)_program [pc + 4] << 8);
              bool capture = _program [pc + 5] > 0;
              if(!Balance(gid, balance_gid, capture, strpos))
                goto Fail;

              strpos = res;					
              pc += _program[pc + 6] | ((int)_program [pc + 7] << 8);
              break;
              }
            case RxOp::Balance: 
              {
              goto Pass;
              }

            case RxOp::Jump:
              pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
              continue;
            case RxOp::TestCharGroup:
              char_group_end = pc + (_program [pc + 1] | ((int)_program [pc + 2] << 8));
              pc += 3;
              continue;
            case RxOp::String:
              start = pc + 2;
              length = _program [pc + 1];
              if (strpos + length > _string_end)
                return false;
              end = start + length;
              for (; start < end; ++start) 
                {
                if (_str [strpos] != _program [start])
                  return false;
                strpos++;
                }
              pc = end;
              continue;
            case RxOp::StringIgnoreCase:
              start = pc + 2;
              length = _program [pc + 1];
              if (strpos + length > _string_end)
                return false;
              end = start + length;
              for (; start < end; ++start) 
                {
                if (_str [strpos] != _program [start] && Char::ToLower (_str [strpos]) != _program [start])
                  return false;
                strpos++;
                }
              pc = end;
              continue;
            case RxOp::StringReverse: 
              {
              start = pc + 2;
              length = _program [pc + 1];
              if (strpos < length)
                return false;
              int p = strpos - length;
              end = start + length;
              for (; start < end; ++start, ++p) 
                {
                if (_str [p] != _program [start])
                  return false;
                }
              strpos -= length;
              pc = end;
              continue;
              }
            case RxOp::StringIgnoreCaseReverse:
              {
              start = pc + 2;
              length = _program [pc + 1];
              if (strpos < length)
                return false;
              int p = strpos - length;
              end = start + length;
              for (; start < end; ++start, ++p) 
                {
                if (_str [p] != _program [start] && Char::ToLower (_str [p]) != _program [start])
                  return false;
                }
              strpos -= length;
              pc = end;
              continue;
              }
            case RxOp::UnicodeString: 
              {
              start = pc + 3;
              length = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              if (strpos + length > _string_end)
                return false;
              end = start + length * 2;
              for (; start < end; start += 2) 
                {
                int c = _program [start] | ((int)_program [start + 1] << 8);
                if (_str [strpos] != c)
                  return false;
                strpos++;
                }
              pc = end;
              continue;
              }
            case RxOp::UnicodeStringIgnoreCase: 
              {
              start = pc + 3;
              length = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              if (strpos + length > _string_end)
                return false;
              end = start + length * 2;
              for (; start < end; start += 2)
                {
                int c = _program [start] | ((int)_program [start + 1] << 8);
                if (_str [strpos] != c && Char::ToLower (_str [strpos]) != c)
                  return false;
                strpos++;
                }
              pc = end;
              continue;
              }
            case RxOp::UnicodeStringReverse: 
              {
              start = pc + 3;
              length = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              if (strpos < length)
                return false;
              int p = strpos - length;
              end = start + length * 2;
              for (; start < end; start += 2, p += 2) 
                {
                int c = _program [start] | ((int)_program [start + 1] << 8);
                if (_str [p] != c)
                  return false;
                }
              strpos -= length;
              pc = end;
              continue;
              }
            case RxOp::UnicodeStringIgnoreCaseReverse: 
              {
              start = pc + 3;
              length = _program [pc + 1] | ((int)_program [pc + 2] << 8);
              if (strpos < length)
                return false;
              int p = strpos - length;
              end = start + length * 2;
              for (; start < end; start += 2, p += 2) {
                int c = _program [start] | ((int)_program [start + 1] << 8);
                if (_str [p] != c && Char::ToLower (_str [p]) != c)
                  return false;
                }
              strpos -= length;
              pc = end;
              continue;
              }

              /*
              * The opcodes below are basically specialized versions of one 
              * generic opcode, which has three parameters:
              * - reverse (Reverse), revert (No), ignore-case (IgnoreCase)
              * Thus each opcode has 8 variants.
              * FIXME: Maybe move all unusual variations 
              * (Reverse+IgnoreCase+Unicode) into a generic GenericChar opcode 
              * like in the old interpreter.
              * FIXME: Move all the Reverse opcodes to a separate method.
              */

              // GENERATED BY gen-interp.cs, DO NOT MODIFY

              /* Char */

            case RxOp::Char:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c == _program [pc + 1]))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;

              /* Range */

            case RxOp::Range:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c >= _program [pc + 1] && c <= _program [pc + 2]))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;

              /* UnicodeRange */

            case RxOp::UnicodeRange:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5;
              continue;

              /* UnicodeChar */

            case RxOp::UnicodeChar:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;

              /* CategoryAny */

            case RxOp::CategoryAny:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c != '\n'))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryAnySingleline */

            case RxOp::CategoryAnySingleline:
              if (strpos < _string_end) 
                {
                // wchar_t c = _str [strpos];
                for( ; ; )
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryWord */

            case RxOp::CategoryWord:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((Char::IsLetterOrDigit (c) || Char::GetUnicodeCategory (c) == UnicodeCategory::ConnectorPunctuation))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryDigit */

            case RxOp::CategoryDigit:
              if (strpos < _string_end) 
                {
                wchar_t c = _str[strpos];
                if(((Char::IsDigit (c)))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryWhiteSpace */

            case RxOp::CategoryWhiteSpace:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (((Char::IsWhiteSpace (c)))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryEcmaWord */

            case RxOp::CategoryEcmaWord:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || L'0' <= c && c <= L'9' || c == L'_'))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryEcmaWhiteSpace */

            case RxOp::CategoryEcmaWhiteSpace:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (((c == L' ' || c == L'\t' || c == L'\n' || c == L'\r' || c == L'\f' || c == L'\v')))
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryUnicodeSpecials */

            case RxOp::CategoryUnicodeSpecials:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (((0xFEFF <= c && c <= 0xFEFF || 0xFFF0 <= c && c <= 0xFFFD))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;

              /* CategoryUnicode */

            case RxOp::CategoryUnicode:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (((Char::GetUnicodeCategory (c) == (UnicodeCategory)_program [pc + 1]))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;

              /* CategoryGeneral */

            case RxOp::CategoryGeneral:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (((CategoryUtils::IsCategory((Category)_program [pc + 1], c)))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;

              /* Bitmap */

            case RxOp::Bitmap:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3 + _program [pc + 2];
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3 + _program [pc + 2];
              continue;

              /* UnicodeBitmap */

            case RxOp::UnicodeBitmap:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
              continue;
            case RxOp::CharIgnoreCase:
              if (strpos < _string_end) 
                {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (((c == _program [pc + 1]))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;
            case RxOp::RangeIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (((c >= _program [pc + 1] && c <= _program [pc + 2]))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::UnicodeRangeIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5;
              continue;
            case RxOp::UnicodeCharIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) 
                  {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::BitmapIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3 + _program [pc + 2];
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3 + _program [pc + 2];
              continue;
            case RxOp::UnicodeBitmapIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos ++;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
              continue;
            case RxOp::NoChar:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((c == _program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoRange:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((c >= _program [pc + 1] && c <= _program [pc + 2]))) 
                  {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeRange:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  pc += 5;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeChar:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) 
                  {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end))
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryAny:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (!((c != '\n')))
                  {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryAnySingleline:
              if (strpos < _string_end) 
                {
                }
              return false;
            case RxOp::NoCategoryWord:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (!((Char::IsLetterOrDigit (c) || Char::GetUnicodeCategory (c) == UnicodeCategory::ConnectorPunctuation))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryDigit:
              if (strpos < _string_end) 
                {
                wchar_t c = _str [strpos];
                if (!((Char::IsDigit (c)))) 
                  {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end))
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryWhiteSpace:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((Char::IsWhiteSpace (c)))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryEcmaWord:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!(('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_'))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end))
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryEcmaWhiteSpace:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryUnicodeSpecials:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((0xFEFF <= c && c <= 0xFEFF || 0xFFF0 <= c && c <= 0xFFFD)))
                  {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryUnicode:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((Char::GetUnicodeCategory (c) == (UnicodeCategory)_program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryGeneral:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                if (!((CategoryUtils::IsCategory ((Category)_program [pc + 1], c)))) 
                  {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoBitmap:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 3 + _program [pc + 2];
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeBitmap:
              if (strpos < _string_end) {
                wchar_t c = _str [strpos];
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCharIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (!((c == _program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoRangeIgnoreCase:
              if (strpos < _string_end) 
                {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (!((c >= _program [pc + 1] && c <= _program [pc + 2]))) 
                  {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) 
                    {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeRangeIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (!((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  pc += 5;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeCharIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                if (!((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoBitmapIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 3 + _program [pc + 2];
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeBitmapIgnoreCase:
              if (strpos < _string_end) {
                wchar_t c = Char::ToLower (_str [strpos]);
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos ++;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::CharReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c == _program [pc + 1]))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;
            case RxOp::RangeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c >= _program [pc + 1] && c <= _program [pc + 2]))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::UnicodeRangeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5;
              continue;
            case RxOp::UnicodeCharReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::CategoryAnyReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c != '\n'))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryAnySinglelineReverse:
              if (strpos > 0) {
                //wchar_t c = _str [strpos - 1];
                for(;;)
                  {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryWordReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((Char::IsLetterOrDigit (c) || Char::GetUnicodeCategory (c) == UnicodeCategory::ConnectorPunctuation))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryDigitReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((Char::IsDigit (c)))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryWhiteSpaceReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((Char::IsWhiteSpace (c)))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryEcmaWordReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if ((('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_'))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryEcmaWhiteSpaceReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryUnicodeSpecialsReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((0xFEFF <= c && c <= 0xFEFF || 0xFFF0 <= c && c <= 0xFFFD)))
                  {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 1;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 1;
              continue;
            case RxOp::CategoryUnicodeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((Char::GetUnicodeCategory (c) == (UnicodeCategory)_program [pc + 1]))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;
            case RxOp::CategoryGeneralReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (((CategoryUtils::IsCategory ((Category)_program [pc + 1], c)))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;
            case RxOp::BitmapReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3 + _program [pc + 2];
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3 + _program [pc + 2];
              continue;
            case RxOp::UnicodeBitmapReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
              continue;
            case RxOp::CharIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (((c == _program [pc + 1]))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 2;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 2;
              continue;
            case RxOp::RangeIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (((c >= _program [pc + 1] && c <= _program [pc + 2]))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::UnicodeRangeIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5;
              continue;
            case RxOp::UnicodeCharIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3;
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3;
              continue;
            case RxOp::BitmapIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 3 + _program [pc + 2];
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 3 + _program [pc + 2];
              continue;
            case RxOp::UnicodeBitmapIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  strpos --;
                  if (char_group_end != 0)
                    goto test_char_group_passed;
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  continue;
                  }
                }
              if (char_group_end == 0)
                return false;
              pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
              continue;
            case RxOp::NoCharReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c == _program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoRangeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c >= _program [pc + 1] && c <= _program [pc + 2]))) {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeRangeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  pc += 5;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeCharReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryAnyReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c != '\n'))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryAnySinglelineReverse:
              if (strpos > 0) 
                {
                }
              return false;
            case RxOp::NoCategoryWordReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((Char::IsLetterOrDigit (c) || Char::GetUnicodeCategory (c) == UnicodeCategory::ConnectorPunctuation))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryDigitReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((Char::IsDigit (c)))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryWhiteSpaceReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((Char::IsWhiteSpace (c)))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryEcmaWordReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!(('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_'))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryEcmaWhiteSpaceReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'))) {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryUnicodeSpecialsReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((0xFEFF <= c && c <= 0xFEFF || 0xFFF0 <= c && c <= 0xFFFD)))
                  {
                  pc += 1;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryUnicodeReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((Char::GetUnicodeCategory (c) == (UnicodeCategory)_program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCategoryGeneralReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                if (!((CategoryUtils::IsCategory ((Category)_program [pc + 1], c))))
                  {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoBitmapReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 3 + _program [pc + 2];
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeBitmapReverse:
              if (strpos > 0) {
                wchar_t c = _str [strpos - 1];
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoCharIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (!((c == _program [pc + 1]))) {
                  pc += 2;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoRangeIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (!((c >= _program [pc + 1] && c <= _program [pc + 2]))) {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeRangeIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (!((c >= (_program [pc + 1] | ((int)_program [pc + 2] << 8))) && (c <= (_program [pc + 3] | ((int)_program [pc + 4] << 8))))) {
                  pc += 5;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeCharIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                if (!((c == (_program [pc + 1] | ((int)_program [pc + 2] << 8))))) {
                  pc += 3;
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoBitmapIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                int c2 = (int)c; c2 -= _program [pc + 1]; length = _program [pc + 2];
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 3 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 3 + _program [pc + 2];
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;
            case RxOp::NoUnicodeBitmapIgnoreCaseReverse:
              if (strpos > 0) {
                wchar_t c = Char::ToLower (_str [strpos - 1]);
                int c2 = (int)c; c2 -= (_program [pc + 1] | ((int)_program [pc + 2] << 8)); length = (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                if (!((c2 >= 0 && c2 < (length << 3) && (_program [pc + 5 + (c2 >> 3)] & (1 << (c2 & 0x7))) != 0))) {
                  pc += 5 + (_program [pc + 3] | ((int)_program [pc + 4] << 8));
                  if (char_group_end == 0 || (pc + 1 == char_group_end)) {
                    strpos --;
                    if (pc + 1 == char_group_end)
                      goto test_char_group_passed;
                    }
                  continue;
                  }
                }
              return false;

              // END OF GENERATED CODE

            case RxOp::Branch: {
              int res = 0;
              if (EvalByteCode (pc + 3, strpos, res)) {
                strpos_result = res;
                return true;
                }
              //Console.WriteLine ("branch offset: {0}", _program [pc + 1] | ((int)_program [pc + 2] << 8));
              pc += _program [pc + 1] | ((int)_program [pc + 2] << 8);
              continue;
              }
            case RxOp::Repeat:
            case RxOp::RepeatLazy: {
              /*
              * Repetation is modelled by two opcodes: Repeat and Until which
              * contain the the qualified regex between them, i.e.:
              * Repeat, <bytecode for the inner regex>, Until, <Tail expr>
              * It is processed as follows: 
              * Repeat, [Until, <inner expr>]*, <Tail>
              * This means that nested quantifiers are processed a bit
              * strangely: when the inner quantifier fails to match, its
              * tail is processed which includes the outer Until.
              *
              * This code is from the old interpreter.cs.
              *
              * FIXME: Rethink this.
              */

              int res = 0;

              _repeat.Set(new RepeatContext(_repeat.Release()			// previous context
                ,ReadInt(_program, pc + 3)		// minimum
                ,ReadInt(_program, pc + 7)		// maximum
                ,(RxOp)_program [pc] == RxOp::RepeatLazy // lazy
                ,pc + 11				// subexpression
                ));

              int until = pc + (_program [pc + 1] | ((int)_program [pc + 2] << 8));
              if (!EvalByteCode (until, strpos, res))
                {
                _repeat.Set(_repeat->Previous());
                return false;
                }

              strpos = res;
              strpos_result = strpos;
              return true;
              }
            case RxOp::Until: {
              RepeatContext* current = _repeat.Get();
              int res = 0;

              //
              // Can we avoid recursion?
              //
              // Backtracking can be forced in nested quantifiers from the tail of this quantifier.
              // Thus, we cannot, in general, use a simple loop on repeat.Expression to handle
              // quantifiers.
              //
              // If 'deep' was unmolested, that implies that there was no nested quantifiers.
              // Thus, we can safely avoid recursion.
              //
              if(_deep == current)
                goto Pass;

              start = current->Start();
              int start_count = current->Count();

              // First match at least 'start' items without backtracking
              while (!current->IsMinimum())
                {
                current->IncrementCount();
                current->Start(strpos);
                _deep = current;
                if (!EvalByteCode(current->Expression(), strpos, res)) 
                  {
                  current->Start(start);
                  current->Count(start_count);
                  goto Fail;
                  }
                strpos = res;
                if (_deep != current)	// recursive mode
                  goto Pass;
                }

              if (strpos == current->Start())
                {
                // degenerate match ... match tail or fail
                _repeat.Set(current->Previous());
                _deep = nullptr;
                if(EvalByteCode (pc + 1, strpos, res)) {
                  strpos = res;
                  goto Pass;
                  }
                _repeat.Set(current);
                goto Fail;
                }

              if (current->IsLazy())
                {
                for (;;)
                  {
                  // match tail first ...
                  _repeat.Set(current->Previous());
                  _deep = nullptr;
                  int cp = Checkpoint();
                  if (EvalByteCode (pc + 1, strpos, res))
                    {
                    strpos = res;
                    goto Pass;
                    }

                  Backtrack (cp);

                  // ... then match more
                  _repeat.Set(current);
                  if (current->IsMaximum())
                    goto Fail;
                  current->IncrementCount();
                  current->Start(strpos);
                  _deep = current;
                  if (!EvalByteCode (current->Expression(), strpos, res)) {
                    current->Start(start);
                    current->Count(start_count);
                    goto Fail;
                    }
                  strpos = res;
                  if (_deep != current)	// recursive mode
                    goto Pass;
                  // Degenerate match: ptr has not moved since the last (failed) tail match.
                  // So, next and subsequent tail matches will fail.
                  if (strpos == current->Start())
                    goto Fail;
                  }
                } 
              else 
                {
                sizet stack_size = _stack.Count();

                // match greedily as much as possible
                while (!current->IsMaximum()) {
                  int cp = Checkpoint ();
                  int old_ptr = strpos;
                  int old_start = current->Start();

                  current->IncrementCount();
                  /*if (trace_rx)
                  Console.WriteLine ("recurse with count {0}.", current.Count);*/
                  current->Start(strpos);
                  _deep = current;
                  if (!EvalByteCode(current->Expression(), strpos, res))
                    {
                    current->DecrementCount();
                    current->Start(old_start);
                    Backtrack(cp);
                    break;
                    }
                  strpos = res;
                  if(_deep != current)
                    {
                    // recursive mode: no more backtracking, truncate the stack
                    _stack.Count(stack_size);
                    goto Pass;
                    }
                  _stack.Push(cp);
                  _stack.Push(old_ptr);

                  // Degenerate match: no point going on
                  if (strpos == current->Start())
                    break;
                  }

                /*if (trace_rx)
                Console.WriteLine ("matching tail: {0} pc={1}", strpos, pc + 1);*/
                // then, match the tail, backtracking as necessary.
                _repeat.Set(current->Previous());
                for (;;) {
                  _deep = nullptr;
                  if (EvalByteCode (pc + 1, strpos, res)) {
                    strpos = res;
                    _stack.Count(stack_size);
                    goto Pass;
                    }
                  if(_stack.Count() == stack_size) {
                    _repeat.Set(current);
                    goto Fail;
                    }

                  current->DecrementCount();
                  strpos = _stack.Pop();
                  Backtrack(_stack.Pop());
                  /*if (trace_rx)
                  Console.WriteLine ("backtracking to {0} expr={1} pc={2}", strpos, current.Expression, pc);*/
                  }
                }
              }

            case RxOp::FastRepeat:
            case RxOp::FastRepeatLazy: {
              /*
              * A FastRepeat is a simplified version of Repeat which does
              * not contain another repeat inside, so backtracking is 
              * easier.
              */
              bool lazy = _program [pc] == (byte)RxOp::FastRepeatLazy;
              int res = 0;
              int tail = pc + (_program [pc + 1] | ((int)_program [pc + 2] << 8));
              start = ReadInt (_program, pc + 3);
              end = ReadInt (_program, pc + 7);
              //Console.WriteLine ("min: {0}, max: {1} tail: {2}", start, end, tail);
              length = 0;

              _deep = nullptr;

              // First match at least 'start' items
              while (length < start) {
                if (!EvalByteCode (pc + 11, strpos,res))
                  return false;
                strpos = res;
                length++;
                }

              if(lazy)
                {
                for(;;)
                  {
                  // Match the tail
                  int cp = Checkpoint ();
                  if (EvalByteCode (tail, strpos, res)) {
                    strpos = res;
                    goto repeat_success;
                    }
                  Backtrack (cp);

                  if (length >= end)
                    return false;

                  // Match an item
                  if (!EvalByteCode (pc + 11, strpos, res))
                    return false;
                  strpos = res;
                  length ++;
                  }
                } 
              else 
                {
                // Then match as many items as possible, recording
                // backtracking information
                sizet old_stack_size = _stack.Count();
                while (length < end) {
                  int cp = Checkpoint ();
                  if (!EvalByteCode (pc + 11, strpos, res))
                    {
                    Backtrack (cp);
                    break;
                    }
                  _stack.Push(cp);
                  _stack.Push(strpos);
                  strpos = res;
                  length++;
                  }	

                if (tail <= pc)
                  throw new Exception ();

                // Then, match the tail, backtracking as necessary.
                for(;;)
                  {
                  if (EvalByteCode (tail, strpos, res))
                    {
                    strpos = res;
                    _stack.Count(old_stack_size);
                    goto repeat_success;
                    }
                  if(_stack.Count() == old_stack_size)
                    return false;

                  // Backtrack
                  strpos = _stack.Pop();
                  Backtrack(_stack.Pop ());
                  /*if (trace_rx)
                  Console.WriteLine ("backtracking to: {0}", strpos);*/
                  }
                }

repeat_success:
              // We matched the tail too so just return
              goto Pass;
              }

            default:
              /*Console.WriteLine ("evaluating: {0} at pc: {1}, strpos: {2}", (RxOp)_program [pc], pc, strpos);*/
              //throw NotSupportedException();
              throw SystemException(L"Not Supported");
            }
          continue;

Pass:
          strpos_result = strpos;
          return true;
Fail:
          return false;
test_char_group_passed:
          pc = char_group_end;
          char_group_end = 0;
          continue;
          } // end of while (true)
        }
      }
    }
  }
