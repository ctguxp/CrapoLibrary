// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.IFormatProvider.h"

namespace System
  {
  class CRAPOCOREDLL_API Double : public Object
    {
    public:
        static const double MaxValue;
        static const double MinValue;
    private:
      double _value;
    public:
      Double();
      Double(double const&);
      ~Double();
      Double& operator = (double const&);
      operator double const& () const;
      virtual uint32 GetHashCode() override;
      virtual String ToString() override;
      String ToString(IFormatProvider*);
      String ToString(String);
      String ToString(String, IFormatProvider*);
    public:
      static double NaN;
    };
  }

