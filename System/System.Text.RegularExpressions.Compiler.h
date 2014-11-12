#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Mark : public Object
        {
        public:
           int Start;
           int End;
           int Previous;

        bool IsDefined() 
          {
          return Start >= 0 && End >= 0;
          }

        int Index()
          {
          return Start < End ? Start : End;
          }

        int Length() 
          {
          return Start < End ? End - Start : Start - End;
          }
        };

      class Compiler
        {
        public:
          Compiler();
          ~Compiler();
        };
      }
    }
  }

