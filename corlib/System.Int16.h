// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Int16 : public Object
    {
    public:
      static const int16 MaxValue = 32767i16;
      static const int16 MinValue = (-32767i16 - 1);
    private:
      int16 _value;
    public:
      Int16();
      Int16(int16 const&);
      ~Int16();
      Int16& operator = (int16 const&);
      operator int16 const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }

