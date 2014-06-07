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
              virtual int GetHashCode(T& obj) override
                {
                return obj.GetHashCode();
                }
              virtual bool Equals(T& x, T& y) override
                {
                return x.Equals(&y);
                }
            };
        public:
          virtual ~EqualityComparer()
            {
            }
          virtual bool Equals(T&, T&) override
            {
            return false;
            }
          virtual int GetHashCode(T&) override
            {
            return 0;
            }
          static EqualityComparer<T>* Default()
            {
            if(_default.Get() == nullptr)
              _default.Set(new EqualityComparer<T>::DefaultComparer());
            return _default.Get();
            }
        protected:
          EqualityComparer()
            {
            }
          virtual int IndexOf(Array<T*> arr, T& value, int startIndex, int endIndex)
            {
            for(int i = startIndex; i < endIndex; ++i)
              {
              if(Equals(*arr[i], value))
                return i;
              }
            return -1;
            }
        private:
          static AutoPtr<EqualityComparer<T>> _default;

        friend class Array<T>;
        };

      template<class T>
      AutoPtr<EqualityComparer<T>> EqualityComparer<T>::_default;
      }
    }
  }

