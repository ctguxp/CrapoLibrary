#pragma once
#include "System.Text.RegualarExpressions.ICompiler.h"
#include "System.Text.RegularExpressions.RxOp.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class RxCompiler : public ICompiler
        {
        public:
          RxCompiler();
          ~RxCompiler();
          virtual void Reset() override;
          virtual IMachineFactory* GetMachineFactory() override;
          virtual void EmitFalse() override;
          virtual void EmitTrue() override;
          virtual void EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse) override;
          virtual void EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse) override;
          virtual void EmitString(String str, bool ignore, bool reverse) override;
          virtual void EmitOpen(int gid) override;
          virtual void EmitClose(int gid) override;
          virtual void EmitBalance() override;
          virtual void EmitReference(int gid, bool ignore, bool reverse) override;
          virtual void EmitInfo(int32 count, int32 min, int32 max) override;
          virtual void EmitBranchEnd() override;
          virtual void EmitAlternationEnd() override;

          virtual void EmitOp(RxOp op, bool negate, bool ignore, bool reverse);
          virtual void EmitOpIgnoreReverse(RxOp op, bool ignore, bool reverse);
        private:
          void Emit(byte val);
          void Emit(uint16 val);
          void Emit(int32 val);
          void Emit(RxOp opcode);
          void MakeRoom(int bytes);
        protected:
          int       _curpos;
          ByteArray _program;
        };
      }
    }
  }
