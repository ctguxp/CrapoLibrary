#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class IMachineFactory
        {
        virtual Collections::IDictionary* Mapping() = 0;
        virtual void Mapping(Collections::IDictionary*) = 0;
        virtual int GroupCount() = 0;
        virtual int Gap() = 0;
        virtual void Gap(int) = 0;
        virtual StringArray NamesMapping() = 0;
        virtual void NamesMapping(StringArray) = 0;
        };
      }
    }
  }