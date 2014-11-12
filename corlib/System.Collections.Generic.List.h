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
		      int32    _size;
		      int32    _version;
        public:
          List();
          List(int32);

          const T& operator [] (int32 idx) const;
          T& operator [] (int32 idx);
  
          void Add(T);
          int32 Capacity();
          void Capacity(int32);
          int32 Count();

           Array<T> ToArray();

        private:
          void GrowIfNeeded(int32);

        };
      }
    }
  }
