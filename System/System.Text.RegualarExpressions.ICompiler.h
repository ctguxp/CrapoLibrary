#pragma once
#include "System.Text.RegularExpressions.IMachineFactory.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class ICompiler
        {
        virtual void Reset() = 0;
        virtual IMachineFactory* GetMachineFactory() = 0;
        virtual void EmitFalse() = 0;
        virtual void EmitTrue() = 0;
        virtual void EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse) = 0;
        // TODO virtual void EmitCategory(Category cat, bool negate, bool reverse) = 0;
        // TODO void EmitNotCategory(Category cat, bool negate, bool reverse);
        virtual void EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse) = 0;
        // TODO void EmitSet(wchar_t lo, BitArray set, bool negate, bool ignore, bool reverse);
        virtual void EmitString(String str, bool ignore, bool reverse) = 0;
        //virtual void EmitPosition(Position pos) = 0;
        virtual void EmitOpen(int gid) = 0;
        virtual void EmitClose(int gid) = 0;
        // TODO void EmitBalanceStart(int gid, int balance, bool capture, LinkRef tail);
        virtual void EmitBalance() = 0;
        virtual void EmitReference(int gid, bool ignore, bool reverse) = 0;
        // TODO void EmitIfDefined(int gid, LinkRef tail);
        // TODO void EmitSub(LinkRef tail);
        // TODO void EmitTest(LinkRef yes, LinkRef tail);
        // TODO void EmitBranch(LinkRef next);
        // TODO void EmitJump(LinkRef target);
        // TODO void EmitRepeat(int min, int max, bool lazy, LinkRef until);
        // TODO void EmitUntil(LinkRef repeat);
        // TODO void EmitIn(LinkRef tail);
        virtual void EmitInfo(int count, int min, int max) = 0;
        // TODO void EmitFastRepeat(int min, int max, bool lazy, LinkRef tail);
        // TODO void EmitAnchor(bool reverse, int offset, LinkRef tail);
        virtual void EmitBranchEnd() = 0;
        virtual void EmitAlternationEnd() = 0;
        // TODO LinkRef NewLink();
        // TODO void ResolveLink(LinkRef link);
        };
      }
    }
  }