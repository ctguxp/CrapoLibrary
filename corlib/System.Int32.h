// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Int32 : public Object
    {
    public:
      static const int32 MaxValue = 2147483647;
      static const int32 MinValue = (-2147483647 - 1);
    public:
      Int32();
      Int32(int32 const&);
      ~Int32();
      Int32& operator = (int32 const&);
      operator int32 const& () const;
      virtual String ToString() override;
      String ToString(String& format);
      virtual uint32 GetHashCode() override;
     private:
      int32 _value;
    };
  }

