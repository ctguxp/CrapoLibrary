#pragma once
#include "System.Text.RegularExpressions.IMachineFactory.h"
#include "System.Text.RegularExpressions.Position.h"
#include "System.Text.RegularExpressions.Category.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class LinkRef : public Object
        {
        public:
          ~LinkRef(){}
        protected:  
          LinkRef(){}
        };

      class ICompiler
        {
        public:
          virtual void Reset() = 0;
          virtual IMachineFactory* GetMachineFactory() = 0;
  
          // instruction emission
          virtual void EmitFalse() = 0;
          virtual void EmitTrue() = 0;

          // character matching
          virtual void EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse) = 0;
          virtual void EmitCategory(Category cat, bool negate, bool reverse) = 0;
          virtual void EmitNotCategory(Category cat, bool negate, bool reverse) = 0;
          virtual void EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse) = 0;
          virtual void EmitSet(wchar_t lo, Collections::BitArray& set, bool negate, bool ignore, bool reverse) = 0;

          // other operators
          virtual void EmitString(String str, bool ignore, bool reverse) = 0;
          virtual void EmitPosition(Position pos) = 0;
          virtual void EmitOpen(int gid) = 0;
          virtual void EmitClose(int gid) = 0;
          virtual void EmitBalanceStart(int gid, int balance, bool capture, LinkRef* tail) = 0;
          virtual void EmitBalance() = 0;
          virtual void EmitReference(int gid, bool ignore, bool reverse) = 0;

          // constructs
          virtual void EmitIfDefined(int gid, LinkRef* tail) = 0;
          virtual void EmitSub(LinkRef* tail) = 0;
          virtual void EmitTest(LinkRef* yes, LinkRef* tail) = 0;
          virtual void EmitBranch(LinkRef* next) = 0;
          virtual void EmitJump(LinkRef* target) = 0;
          virtual void EmitRepeat(int min, int max, bool lazy, LinkRef* until) = 0;
          virtual void EmitUntil(LinkRef* repeat) = 0;
          virtual void EmitIn(LinkRef* tail) = 0;
          virtual void EmitInfo(int count, int min, int max) = 0;
          virtual void EmitFastRepeat(int min, int max, bool lazy, LinkRef* tail) = 0;
          virtual void EmitAnchor(bool reverse, int offset, LinkRef* tail) = 0;

          // event for the CILCompiler
          // virtual void EmitBranchEnd() = 0;
          // virtual void EmitAlternationEnd() = 0;

          virtual LinkRef* NewLink() = 0;
          virtual void ResolveLink(LinkRef* link) = 0;
        };
      }
    }
  }