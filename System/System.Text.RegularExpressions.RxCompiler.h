#pragma once
#include "System.Text.RegularExpressions.ICompiler.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"
#include "System.Text.RegularExpressions.RxOp.h"
#include "System.Text.RegularExpressions.RxInterpreter.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class RxLinkRef : public LinkRef
        {
        public:
          IntArray _offsets;
          int      _current;
        public:
          RxLinkRef();
          ~RxLinkRef();
          void PushInstructionBase(int offset);
          void PushOffsetPosition(int offset);
        };

      class RxInterpreterFactory : public IMachineFactory
        {
        private:
          ByteArray                 _program;
          StringArray               _namesMapping;
		      int32                     _gap;
          Collections::IDictionary* _mapping;
          EvalDelegate              _eval_del;
        public:
          RxInterpreterFactory(ByteArray& program, EvalDelegate eval_del);
          virtual int32 Gap() override;
          virtual void Gap(int32 value) override;
          virtual int32 GroupCount() override;
          virtual Collections::IDictionary* Mapping() override;
          virtual void Mapping(Collections::IDictionary* value) override;
          virtual StringArray& NamesMapping() override;
          virtual void NamesMapping(StringArray&) override; 
        };

      class RxCompiler : public ICompiler
        {
        public:
          RxCompiler();
          virtual ~RxCompiler();
          virtual void Reset() override;
          virtual IMachineFactory* GetMachineFactory() override;
          virtual void EmitFalse() override;
          virtual void EmitTrue() override;
          virtual void EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse) override;
          virtual void EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse) override;
          virtual void EmitString(String str, bool ignore, bool reverse) override;
          virtual void EmitPosition(Position pos) override;
          virtual void EmitOpen(int gid) override;
          virtual void EmitClose(int gid) override;
          virtual void EmitBalanceStart(int gid, int balance, bool capture, LinkRef* tail) override;
          virtual void EmitBalance() override;
          virtual void EmitReference(int gid, bool ignore, bool reverse) override;
          virtual void EmitIfDefined(int gid, LinkRef* tail) override;
          virtual void EmitSub(LinkRef* tail) override;
          virtual void EmitTest(LinkRef* yes, LinkRef* tail) override;
          virtual void EmitBranch(LinkRef* next) override;
          virtual void EmitJump(LinkRef* target) override;
          virtual void EmitRepeat(int min, int max, bool lazy, LinkRef* until) override;
          virtual void EmitInfo(int32 count, int32 min, int32 max) override;
          virtual void EmitAnchor(bool reverse, int offset, LinkRef* tail) override;
          virtual void EmitBranchEnd() override;
          virtual void EmitAlternationEnd() override;
          virtual LinkRef* NewLink() override;
          virtual void ResolveLink(LinkRef* link) override;

          virtual void EmitOp(RxOp op, bool negate, bool ignore, bool reverse);
          virtual void EmitOpIgnoreReverse(RxOp op, bool ignore, bool reverse);
        private:
          void BeginLink(LinkRef* lref);
          void EmitLink(LinkRef* lref);
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
