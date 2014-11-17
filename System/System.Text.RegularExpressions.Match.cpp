#include "pch.h"
#include "System.Text.RegularExpressions.Match.h"
#include "System.Text.RegularExpressions.Regex.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {

      SharedPtr<Match> Match::_empty(new Match());

      Match::Match()
        :_regex(nullptr)
        ,_machine(nullptr)
        ,_text_length(0)
        ,_groups(0, 0)
        {
        }

      Match::Match(Regex* regex, IMachine* machine, String text, int text_length, int index, int length)
        :Group(text, index, length)
        ,_regex(regex)
        ,_machine(machine)
        ,_text_length(text_length)
        ,_groups(0, 0)
        {
        }

      Match::Match(Regex* regex, IMachine* machine, String text, int text_length, int n_groups, int index, int length, int n_caps)
        :Group(text, index, length, n_caps)
        ,_regex(regex)
        ,_machine(machine)
        ,_text_length(text_length)
        ,_groups(n_groups, regex->Gap())
        {
        _groups.SetValue(this, 0);
        }

      Match::~Match()
        {
        }

      GCMatch Match::Empty()
        {
        return _empty;
        }
      }
    }
  }
