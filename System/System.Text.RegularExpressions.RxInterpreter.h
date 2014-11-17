#pragma once
#include "System.Text.RegularExpressions.BaseMachine.h"
#include "System.Text.RegularExpressions.Compiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Group;
      class RxInterpreter;

      typedef bool (*EvalDelegate)(RxInterpreter*, int, int&);
     
      class RxInterpreter : public BaseMachine
        {
        private:
          class IntStack
          {
          private:
            IntArray _values;
			      int32     _count;
          public:
            IntStack();
            int32 Pop();
            void Push(int32 value);
            int32 Count();
            void Count(int32 value);
          };
          class RepeatContext
          {
          private:
            int             _start;
			      int             _min;
            int             _max;
			      bool            _lazy;
			      int             _expr_pc;
			      RepeatContext*  _previous;
			      int             _count;
          public:
            RepeatContext(RepeatContext* previous, int min, int max, bool lazy, int expr_pc);
            ~RepeatContext();
            int Start() { return _start; }
            void Start(int value) { _start = value; }
            int Min() { return _min; }
            int Max() { return _max; }
            int Count() { return _count; }
            void Count(int value) { _start = value; }
            void IncrementCount() { ++_count; }
            void DecrementCount() { --_count; }
            bool IsMinimum() { return _min <= _count; }
            bool IsMaximum() { return _max <= _count; }
            bool IsLazy() { return _lazy; }
            int Expression() { return _expr_pc; }
            RepeatContext*  Previous() { return _previous; }
          };
        private:
          ByteArray    _program;
		      String       _str;
		      int          _string_start;
		      int          _string_end;
		      int          _group_count;
          IntArray     _groups;
		      EvalDelegate _eval_del;
          Array<Mark>  _marks;      // mark stack
          sizet        _mark_start; // start of current checkpoint
		      sizet        _mark_end;   // end of checkpoint/next free mark
          IntStack               _stack;
          AutoPtr<RepeatContext> _repeat;
          RepeatContext*         _deep;
        public:
          RxInterpreter(ByteArray /*program*/, EvalDelegate eval_del = nullptr);
          ~RxInterpreter();
          GCMatch Scan(Regex* regex, String text, int start, int end) override;
        private:
          GCMatch GenerateMatch (Regex* regex);
          bool EvalByteCode(int pc, int strpos, int& strpos_result);
          void GetGroupInfo(int gid, int& first_mark_index, int& n_caps);
          void PopulateGroup(Group* g, int first_mark_index, int n_caps);
          void ResetGroups();
          int GetLastDefined(int gid);
          void Open(int gid, int ptr);
          int CreateMark(int previous);
          void Close(int gid, int ptr);
          bool Balance(int gid, int balance_gid, bool capture, int ptr);
          void Backtrack(int cp);
          int Checkpoint();
          static bool IsWordChar(wchar_t c);
          static int ReadInt(ByteArray& code, int pc);
        };
      }
    }
  }
