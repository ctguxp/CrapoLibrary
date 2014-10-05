#pragma once
#include "System.Text.RegularExpressions.Capture.h"
#include "System.Text.RegularExpressions.CaptureCollection.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Group : public Capture
        {
        private:
          static AutoPtr<Group> _fail;
        private:
          bool              _success;
		      CaptureCollection _captures;
        public:
          Group();
          Group(String text, int index, int length);
          Group(String text, int index, int length, int n_caps);
          ~Group();
          CaptureCollection& Captures() { return _captures; }
          bool Success();
          static Group* Fail();
        };
      }
    }
  }
