// Perch Lake Computer System
// Golden on 6/26/2014

#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Capture : public Object
        {
        private:
          int32  _index;
          int32  _length;
          String _text;
        public:
          Capture();
          Capture(String /*text*/, int32 index = 0, int32 length = 0);
          ~Capture();
          String Value();
          virtual String ToString() override;
        protected:
          int32 Index();
          int32 Length();
          String Text();
        };
      }
    }
  }
