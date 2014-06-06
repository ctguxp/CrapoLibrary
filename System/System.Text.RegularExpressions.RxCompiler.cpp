#include "pch.h"
#include "System.Text.RegularExpressions.RxCompiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
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
        // TODO return new RxInterpreterFactory(dst, null);
        throw NotImplementedException();
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

      // Private functions
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
