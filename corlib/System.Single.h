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
      virtual bool Equals(Object* obj) override;
      virtual uint32 GetHashCode() override;
      static bool IsNaN(float);
      virtual String ToString() override;
      String ToString(IFormatProvider*);
      String ToString(String);
      String ToString(String, IFormatProvider*);
      static float Parse(String s, IFormatProvider* provider);
    public:
      static const float NaN;
      static const float MaxValue;
      static const float MinValue;
      static const double MaxValueEpsilon;
    };
  }

