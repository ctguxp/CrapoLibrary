#pragma once
#include "System.Collections.Generic.EqualityComparer.h"
#include "System.Int32.h"
#include "System.Collections.HashPrimeNumbers.h"

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {

      class Link : public Object
        {
        public:
        int HashCode;
        int Next;
        };

      static const float DEFAULT_LOAD_FACTOR = ((float)90 / 100);

      template<class TKey, class TValue>
      class Dictionary
        {
        enum
          {
          INITIAL_SIZE = 10,
          NO_SLOT = -1,
          HASH_FLAG = Int32::MinValue
          };
        public:
          Dictionary();  
          ~Dictionary();
          void Add(TKey key, TValue value);
          bool ContainsKey(TKey key);
          int Count();
          bool TryGetValue(TKey key, TValue& value);
        private:
          void Init(uint32 capacity, IEqualityComparer<TKey>* hcp);       
          void InitArrays(uint32 size);
          void Resize();
        private:
          IntArray                 _table;
          Array<Link>              _linkSlots;
          Array<TKey>              _keySlots;
          Array<TValue>            _valueSlots;
          IEqualityComparer<TKey>* _hcp;
          int                      _touchedSlots;
          int                      _emptySlot;
          int                      _count;
          int                      _threshold;
          int                      _generation;
        };
      }
    }
  }

#include "System.Collections.Generic.Dictionary.hpp"