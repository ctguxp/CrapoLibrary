#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class RxInterpreter;

      typedef bool (*EvalDelegate)(RxInterpreter*, int, int&);
      //internal delegate bool EvalDelegate (RxInterpreter interp, int strpos, ref int strpos_result);
      
      class RxInterpreter
        {
        public:
          RxInterpreter();
          ~RxInterpreter();
        };
      }
    }
  }
