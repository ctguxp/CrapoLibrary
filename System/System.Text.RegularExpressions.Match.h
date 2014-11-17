#pragma once
#include "System.Text.RegularExpressions.Group.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"
#include "System.Text.RegularExpressions.GroupCollection.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Regex;
      class Match : public Group
        {
        private:
          static SharedPtr<Match> _empty;
        private:
          Regex*          _regex;
		      IMachine*       _machine;
		      int             _text_length;
          GroupCollection _groups;
        public:
          Match();
          Match(Regex* /*regex*/, IMachine* /*machine*/, String /*text*/, int /*text_length*/, int /*index*/, int /*length*/);
          Match(Regex* /*regex*/, IMachine* /*machine*/, String /*text*/, int /*text_length*/, int /*n_groups*/, int /*index*/, int /*length*/, int /*n_caps*/);
          ~Match();
          static GCMatch Empty();
          GroupCollection& Groups() { return _groups; }
        };
      typedef SharedPtr<Match> GCMatch;
      }
    }
  }
