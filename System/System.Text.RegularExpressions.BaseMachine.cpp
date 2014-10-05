#include "pch.h"
#include "System.Text.RegularExpressions.BaseMachine.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      BaseMachine::BaseMachine()
        :_needs_groups_or_captures(true)
        {
        }
      BaseMachine::~BaseMachine()
        {
        }
      }
    }
  }
