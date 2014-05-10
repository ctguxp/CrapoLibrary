#pragma once

namespace System
  {
  namespace Collections
    {
    class IHashCodeProvider
      {
      public:
        virtual bool GetHashCode(Object*) = 0;
      };
    }
  }
