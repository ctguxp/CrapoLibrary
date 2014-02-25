#pragma once

namespace System
  {
  namespace Collections
    {
    namespace Generic
      {

      template<class TKey, class TValue>
      Dictionary<TKey, TValue>::Dictionary()
        :_table()
        ,_linkSlots()
        ,_keySlots()
        ,_valueSlots()
        ,_hcp(nullptr)
        ,_touchedSlots(0)
        ,_emptySlot(NO_SLOT)
        ,_count(0)
        ,_threshold(0)
        ,_generation(0)
        {
        Init(INITIAL_SIZE, nullptr);
        }

      template<class TKey, class TValue>
      Dictionary<TKey, TValue>::~Dictionary()
        {
        if(_hcp != nullptr)
          {
          delete _hcp;
          _hcp = nullptr;
          }
        }

      template<class TKey, class TValue>
      int Dictionary<TKey, TValue>::Count()
        {
			  return _count;
		    }


      template<class TKey, class TValue>
      void Dictionary<TKey, TValue>::Init(uint32 capacity, IEqualityComparer<TKey>* hcp)
        {
        _hcp = (hcp != nullptr) ? hcp : EqualityComparer<TKey>::Default();
        if(capacity == 0)
          capacity = INITIAL_SIZE;

        // Modify capacity so 'capacity' elements can be added without resizing
        capacity = (uint32)(capacity / DEFAULT_LOAD_FACTOR) + 1;

        InitArrays(capacity);
        }

      template<class TKey, class TValue>
      void Dictionary<TKey, TValue>::InitArrays(uint32 size)
        {
        _table.Length(size);
        _table.Set(0);

        _linkSlots.Length(size);

        _keySlots.Length(size);
        _valueSlots.Length(size);

        _threshold = (int)(_table.Length() * DEFAULT_LOAD_FACTOR);
        if(_threshold == 0 && _table.Length() > 0)
          _threshold = 1;
        }

      template<class TKey, class TValue>
      bool Dictionary<TKey, TValue>::ContainsKey(TKey key)
        {
        //if (key == null)
        //throw new ArgumentNullException ("key");

        // get first item of linked list corresponding to given key
        int hashCode = _hcp->GetHashCode(key) | HASH_FLAG;
        int cur = _table[(hashCode & Int32::MaxValue) % _table.Length()] - 1;

        // walk linked list until right slot is found or end is reached
        while (cur != NO_SLOT)
          {
          // The ordering is important for compatibility with MS and strange
          // Object.Equals () implementations
          if(_linkSlots [cur].HashCode == hashCode && _hcp->Equals(_keySlots[cur], key))
            return true;
          cur = _linkSlots[cur].Next;
          }

        return false;
        }

      template<class TKey, class TValue>
      void Dictionary<TKey, TValue>::Resize()
        {
        // From the SDK docs:
        //	 Hashtable is automatically increased
        //	 to the smallest prime number that is larger
        //	 than twice the current number of Hashtable buckets
        int newSize = HashPrimeNumbers::ToPrime((_table.Length() << 1) | 1);

        // allocate new hash table and link slots array
        IntArray newTable(newSize);
        Array<Link> newLinkSlots(newSize);

        for(int i = 0; i < (int)_table.Length(); i++) 
          {
          int cur = _table[i] - 1;
          while(cur != NO_SLOT) 
            {
            int hashCode = newLinkSlots[cur].HashCode = _hcp->GetHashCode(_keySlots[cur]) | HASH_FLAG;
            int index = (hashCode & Int32::MaxValue) % newSize;
            newLinkSlots[cur].Next = newTable[index] - 1;
            newTable[index] = cur + 1;
            cur = _linkSlots[cur].Next;
            }
          }
        _table = newTable;
        _linkSlots = newLinkSlots;

        // allocate new data slots, copy data
        _keySlots.Length(newSize);
        _valueSlots.Length(newSize);

        _threshold = (int)(newSize * DEFAULT_LOAD_FACTOR);
        }

      template<class TKey, class TValue>
      void Dictionary<TKey, TValue>::Add(TKey key, TValue value)
            {
            // get first item of linked list corresponding to given key
            int32 hashCode = _hcp->GetHashCode(key) | HASH_FLAG;
            int index = (hashCode & Int32::MaxValue) % _table.Length();
            int cur = _table[index] - 1;

            // walk linked list until end is reached (throw an exception if a
            // existing slot is found having an equivalent key)
            while(cur != NO_SLOT) 
              {
              // The ordering is important for compatibility with MS and strange
              // Object.Equals () implementations
              if(_linkSlots[cur].HashCode == hashCode && _hcp->Equals(_keySlots[cur], key))
                throw ArgumentException(L"An element with the same key already exists in the dictionary.");
              cur = _linkSlots[cur].Next;
              }

            if(++_count > _threshold) 
              {
              Resize();
              index = (hashCode & Int32::MaxValue) % _table.Length();
              }

            // find an empty slot
            cur = _emptySlot;
            if(cur == NO_SLOT)
              cur = _touchedSlots++;
            else 
              _emptySlot = _linkSlots[cur].Next;

            // store the hash code of the added item,
            // prepend the added item to its linked list,
            // update the hash table
            _linkSlots[cur].HashCode = hashCode;
            _linkSlots[cur].Next = _table[index] - 1;
            _table[index] = cur + 1;

            // store item's data 
            _keySlots[cur] = key;
            _valueSlots[cur] = value;

            _generation++;
            }
      }
    }
  }