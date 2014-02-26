#pragma once
#include "System.Object.h"
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Boolean : public Object
    {
    private:
      bool _value;
    public:
      Boolean();
      Boolean(bool const&);
      ~Boolean();
      Boolean& operator = (bool const&);
      operator bool const& () const;
      virtual String ToString() override;
      virtual uint32 GetHashCode() override;
    };
  }
