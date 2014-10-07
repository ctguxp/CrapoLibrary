#include "pch.h"
#include "System.Text.RegularExpressions.RxCompiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      RxLinkRef::RxLinkRef()
        :_offsets(8)
        ,_current(0)
        {
        }
      RxLinkRef::~RxLinkRef()
        {
        }
      void RxLinkRef::PushInstructionBase(int offset)
        {
        if((_current & 1) != 0)
          throw Exception();
        if(_current == (int)_offsets.Length())
          {
          _offsets.Length(_offsets.Length() * 2);
          }
        _offsets[_current++] = offset;
        }
      void RxLinkRef::PushOffsetPosition(int offset)
        {
        if((_current & 1) == 0)
          throw Exception();
        _offsets[_current++] = offset;
        }

      RxInterpreterFactory::RxInterpreterFactory(ByteArray& program, EvalDelegate eval_del)
        :_program(program)
        ,_namesMapping()
        ,_gap(0)
        ,_mapping(nullptr)
        ,_eval_del(eval_del)
        {
        }
      int32 RxInterpreterFactory::GroupCount()
        { 
        return (int32)_program[1] | ((int32)_program[2] << 8);
        }
      StringArray& RxInterpreterFactory::NamesMapping()
        { 
        return _namesMapping;
        }
      void RxInterpreterFactory::NamesMapping(StringArray& value)
        { 
        _namesMapping = value; 
        }
      int32 RxInterpreterFactory::Gap()
        {
        return _gap;
        }
      void RxInterpreterFactory::Gap(int32 value)
        {
        _gap = value;
        }
      Collections::IDictionary* RxInterpreterFactory::Mapping()
        {
        return _mapping;
        }
      void RxInterpreterFactory::Mapping(Collections::IDictionary* value)
        {
        _mapping = value;
        }
      IMachine* RxInterpreterFactory::NewInstance()
        {
        return new RxInterpreter(_program, _eval_del);
        }

      RxCompiler::RxCompiler()
        :_curpos(0)
        ,_program(32)
        {
        }
      RxCompiler::~RxCompiler()
        {
        }

      // Overrides
      void RxCompiler::Reset()
        {
        _curpos = 0;
        }
      IMachineFactory* RxCompiler::GetMachineFactory()
        {
        ByteArray dst(_program);
        dst.Length(_curpos);
        return new RxInterpreterFactory(dst, nullptr);
        }
      void RxCompiler::EmitFalse()
        {
        Emit(RxOp::False);
        }
      void RxCompiler::EmitTrue()
        {
        Emit(RxOp::True);
        }
      void RxCompiler::EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse)
        {
        if(ignore)
          {
          c = Char::ToLower(c);
          }
        if(c < 256)
          {
          EmitOp(RxOp::Char, negate, ignore, reverse);
          Emit((byte)c);
          }
        else
          {
          EmitOp(RxOp::UnicodeChar, negate, ignore, reverse);
          Emit((uint16)c);
          }
        }
      void RxCompiler::EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse)
        {
        if(lo < 256 && hi < 256)
          {
          EmitOp(RxOp::Range, negate, ignore, reverse);
          Emit((byte)lo);
          Emit((byte)hi);
          }
        else
          {
          EmitOp(RxOp::UnicodeRange, negate, ignore, reverse);
          Emit((uint16)lo);
          Emit((uint16)hi);
          }
        }
      void RxCompiler::EmitString(String str, bool ignore, bool reverse)
        {
        bool flag = false;
        int num = 0;
        if(ignore)
          {
          num++;
          }
        if(reverse)
          {
          num += 2;
          }
        if(ignore)
          {
          str = str.ToLower();
          }
        if(str.Length() < 256)
          {
          flag = true;
          for(int i = 0; i < str.Length(); i++)
            {
            if(str[i] >= 256)
              {
              flag = false;
              break;
              }
            }
          }
        if(flag)
          {
          EmitOpIgnoreReverse(RxOp::String, ignore, reverse);
          Emit((byte)str.Length());
          for(int i = 0; i < str.Length(); i++)
            {
            Emit((byte)str[i]);
            }
          }
        else
          {
          EmitOpIgnoreReverse(RxOp::UnicodeString, ignore, reverse);
          if(str.Length() > 65535)
            {
            //throw NotSupportedException();
            throw SystemException(L"Not Supported");
            }
          Emit((uint16)str.Length());
          for(int i = 0; i < str.Length(); i++)
            {
            Emit((uint16)str[i]);
            }
          }
        }
      void RxCompiler::EmitPosition(Position pos)
        {
        switch (pos)
          {
          case Position::Any:
            Emit(RxOp::AnyPosition);
            break;
          case Position::Start:
            Emit(RxOp::StartOfString);
            break;
          case Position::StartOfString:
            Emit(RxOp::StartOfString);
            break;
          case Position::StartOfLine:
            Emit(RxOp::StartOfLine);
            break;
          case Position::StartOfScan:
            Emit(RxOp::StartOfScan);
            break;
          case Position::End:
            Emit(RxOp::End);
            break;
          case Position::EndOfString:
            Emit(RxOp::EndOfString);
            break;
          case Position::EndOfLine:
            Emit(RxOp::EndOfLine);
            break;
          case Position::Boundary:
            Emit(RxOp::WordBoundary);
            break;
          case Position::NonBoundary:
            Emit(RxOp::NoWordBoundary);
            break;
          default:
            //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
          }
        }
      void RxCompiler::EmitOpen(int gid)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit(RxOp::OpenGroup);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitClose(int gid)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit(RxOp::CloseGroup);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitBalanceStart(int gid, int balance, bool capture, LinkRef* tail)
        {
        BeginLink(tail);
        Emit(RxOp::BalanceStart);
        Emit((uint16)gid);
        Emit((uint16)balance);
        Emit((byte)(capture ? 1 : 0));
        EmitLink(tail);
        }
      void RxCompiler::EmitBalance()
        {
        Emit(RxOp::Balance);
        }
      void RxCompiler::EmitReference(int gid, bool ignore, bool reverse)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        EmitOpIgnoreReverse(RxOp::Reference, ignore, reverse);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitIfDefined(int gid, LinkRef* tail)
        {
        if(gid > UInt16::MaxValue)
          //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
        BeginLink(tail);
        Emit(RxOp::IfDefined);
        EmitLink(tail);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitSub(LinkRef* tail)
        {
        BeginLink(tail);
        Emit(RxOp::SubExpression);
        EmitLink(tail);
        }
      void RxCompiler::EmitTest(LinkRef* yes, LinkRef* tail)
        {
        BeginLink(yes);
        BeginLink(tail);
        Emit(RxOp::Test);
        EmitLink(yes);
        EmitLink(tail);
        }
      void RxCompiler::EmitBranch(LinkRef* next)
        {
        BeginLink(next);
        Emit(RxOp::Branch);
        EmitLink(next);
        }
      void RxCompiler::EmitJump(LinkRef* target)
        {
        BeginLink(target);
        Emit(RxOp::Jump);
        EmitLink(target);
        }
      void RxCompiler::EmitRepeat(int min, int max, bool lazy, LinkRef* until)
        {
        BeginLink(until);
        Emit(lazy ? RxOp::RepeatLazy : RxOp::Repeat);
        EmitLink(until);
        Emit(min);
        Emit(max);
        }
      void RxCompiler::EmitInfo(int32 count, int32 min, int32 max)
        {
        Emit(RxOp::Info);
        if(count > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit((uint16)count);
        Emit(min);
        Emit(max);
        }
      void RxCompiler::EmitAnchor(bool reverse, int offset, LinkRef* tail)
        {
        BeginLink(tail);
        if(reverse)
          Emit(RxOp::AnchorReverse);
        else
          Emit(RxOp::Anchor);
        EmitLink(tail);
        if (offset > UInt16::MaxValue)
          //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
        Emit((uint16)offset);
        }
      void RxCompiler::EmitBranchEnd()
        {
        }
      void RxCompiler::EmitAlternationEnd()
        {
        }
      // Virtual functions
      void RxCompiler::EmitOp(RxOp op, bool negate, bool ignore, bool reverse)
        {
        int num = 0;
        if(negate)
          {
          num++;
          }
        if(ignore)
          {
          num += 2;
          }
        if(reverse)
          {
          num += 4;
          }
        Emit((byte)((byte)op + (byte)num));
        }
      void RxCompiler::EmitOpIgnoreReverse(RxOp op, bool ignore, bool reverse)
        {
        int num = 0;
        if(ignore)
          {
          num++;
          }
        if(reverse)
          {
          num += 2;
          }
        Emit((byte)((byte)op + (byte)num));
        }
      LinkRef* RxCompiler::NewLink()
        {
        return new RxLinkRef();
        }
      void RxCompiler::ResolveLink(LinkRef* link)
        {
        RxLinkRef& l = static_cast<RxLinkRef&>(*link);
        for (int i = 0; i < l._current; i += 2)
          {
          int offset = _curpos - l._offsets[i];
          if (offset > UInt16::MaxValue)
            //throw new NotSupportedException();
              throw SystemException(L"Not Supported");
          int offsetpos = l._offsets [i + 1];
          _program[offsetpos] = (byte)offset;
          _program[offsetpos + 1] = (byte)(offset >> 8);
          }
        }

      // Private functions
      void RxCompiler::BeginLink(LinkRef* lref)
        {
        RxLinkRef& link = static_cast<RxLinkRef&>(*lref);
        link.PushInstructionBase(_curpos);
        }
      void RxCompiler::EmitLink(LinkRef* lref)
        {
        RxLinkRef& link = static_cast<RxLinkRef&>(*lref);
        link.PushOffsetPosition(_curpos);
        Emit((uint16)0);
        }
      void RxCompiler::Emit(byte val)
        {
        MakeRoom(1);
        _program[_curpos] = val;
        _curpos++;
        }
      void RxCompiler::Emit(uint16 val)
        {
        MakeRoom(2);
        _program[_curpos] = (byte)val;
        _program[_curpos + 1] = (byte)(val >> 8);
        _curpos += 2;
        }
      void RxCompiler::Emit(int32 val)
        {
        MakeRoom(4);
        _program[_curpos] = (byte)val;
        _program[_curpos + 1] = (byte)(val >> 8);
        _program[_curpos + 2] = (byte)(val >> 16);
        _program[_curpos + 3] = (byte)(val >> 24);
        _curpos += 4;
        }
      void RxCompiler::Emit(RxOp opcode)
        {
        Emit((byte)opcode);
        }
      void RxCompiler::MakeRoom(int bytes)
        {
        while(_curpos + bytes > (int)_program.Length())
          {
          _program.Length(_program.Length() * 2);
          }
        }
      }
    }
  }
