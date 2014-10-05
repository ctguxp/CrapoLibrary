#pragma once
#include "System.Text.RegularExpressions.Capture.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class CaptureCollection : public Collections::ICollection
        {
        private:
          Array<Capture*> _list;
        public:
          CaptureCollection(sizet /*n*/);
          virtual ~CaptureCollection();
          virtual sizet Count() override;
          virtual bool IsSynchronized() override;
          virtual Collections::IEnumerator* GetEnumerator() override;
          void SetValue(Capture* cap, sizet i);
        };
      }
    }
  }
