#pragma once
#include "System.Collections.Generic.IEqualityComparer.h"
#include "AutoPtr.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template<class T>
      class EqualityComparer : public IEqualityComparer<T>
        {
        private:
          class DefaultComparer : public EqualityComparer<T>
            {
            public:
              virtual int GetHashCode (T obj) override
                {
                return obj.GetHashCode();
                }
              virtual bool Equals (T x, T y)
                {
                return x.Equals(&y);
                }
            };
        public:
          EqualityComparer()
            {
            }
          virtual bool Equals(T, T) override
            {
            return false;
            }
          virtual int GetHashCode(T) override
            {
            return 0;
            }
          static EqualityComparer<T>* Default()
            {
            return _default;
            }
        private:
          static EqualityComparer<T>* _default;
        };

      template<class T>
      EqualityComparer<T>* EqualityComparer<T>::_default = new EqualityComparer<T>::DefaultComparer();
      }
    }
  }

