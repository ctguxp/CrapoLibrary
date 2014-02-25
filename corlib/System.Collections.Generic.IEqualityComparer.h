#pragma once

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template<class T>
      class IEqualityComparer
        {
        public:
        virtual bool Equals(T x, T y) = 0;
		    virtual int GetHashCode(T obj) = 0;
        };
      }
    }
  }