#pragma once
#include "System.Text.RegularExpressions.IMachineFactory.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class BaseMachine : public IMachine
        {
        protected:
          // Specify whenever Match objects created by this machine need to be fully
		      // built. If false, these can be omitted, avoiding some memory allocations and
		      // processing time.
          bool _needs_groups_or_captures; 
        public:
          BaseMachine();
          virtual ~BaseMachine();
        };
      }
    }
  }
