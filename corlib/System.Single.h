// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.IFormatProvider.h"

namespace System
  {
  class CRAPOCOREDLL_API Single : public Object
    {
    private:
      float _value;
    public:
      Single();
      Single(float const&);
      ~Single();
      Single& operator = (float const&);
      operator float const& () const;
      virtual uint32 GetHashCode() override;
      virtual String ToString() override;
      String ToString(IFormatProvider*);
      String ToString(String);
      String ToString(String, IFormatProvider*);
    public:
      static float NaN;
    };
  }

