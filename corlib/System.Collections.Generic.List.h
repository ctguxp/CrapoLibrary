#pragma once
#include "System.Array.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {
      template<class T>
      class List
        {
        enum
          {
          DefaultCapacity = 4
          };
        private:
          Array<T> _items;
		      uintptr  _size;
		      int      _version;
        public:
          List();
          List(uintptr);

          const T& operator [] (sizet idx) const;
          T& operator [] (sizet idx);
  
          void Add(T);
          uintptr Capacity();
          void Capacity(uintptr);
          uintptr Count();

           Array<T> ToArray();

        private:
          void GrowIfNeeded(uintptr);

        };
      }
    }
  }
