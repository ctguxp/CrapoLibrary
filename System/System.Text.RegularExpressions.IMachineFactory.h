#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class IMachineFactory : public Object
        {
        public:
        virtual Collections::IDictionary* Mapping() = 0;     // get
        virtual void Mapping(Collections::IDictionary*) = 0; // set
        virtual int GroupCount() = 0;                        // get
        virtual int Gap() = 0;                               // get
        virtual void Gap(int) = 0;                           // set
        virtual StringArray NamesMapping() = 0;              // get
        virtual void NamesMapping(StringArray) = 0;          // set
        };
      }
    }
  }