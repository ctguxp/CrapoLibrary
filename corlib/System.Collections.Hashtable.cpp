#include "pch.h"
#include "System.Collections.Hashtable.h"
#include "System.Exception.h"
#include "System.Single.h"
#include "System.Collections.HashPrimeNumbers.h"
#include "System.Array.hpp"

namespace System
  {
  namespace Collections
    {
    Hashtable::KeyMarker Hashtable::Removed;

    Hashtable::Hashtable(int capacity, float loadFactor, IHashCodeProvider* hcp, IComparer* comparer)
      :_inUse(0)
      ,_modificationCount(0)
      ,_loadFactor()
      ,_threshold()
      ,_hcpRef(hcp)
      ,_comparerRef(comparer)
      ,_equalityComparer(nullptr)
      ,_table()
      ,_hashes()
      {
      Init(capacity, loadFactor);
      }

    Hashtable::Hashtable(IDictionary* d, float loadFactor, IHashCodeProvider* hcp, IComparer* comparer)
      :_inUse(0)
      ,_modificationCount(0)
      ,_loadFactor()
      ,_threshold()
      ,_hcpRef(hcp)
      ,_comparerRef(comparer)
      ,_equalityComparer(nullptr)
      ,_table()
      ,_hashes()
      {
      Init((d == nullptr) ? 0 : d->Count(), loadFactor);
      if(d == nullptr)
        throw ArgumentException(L"dictionary");
      }

    Hashtable::~Hashtable()
      {
      int i = 0;
      for(;i < _table.Length(); ++i)
        {
        if(_table[i].key != nullptr)
          delete _table[i].key;
        if(_table[i].value != nullptr)
          delete _table[i].value;
        }
      }

    int Hashtable::Count()
      {
      return _inUse;
      }

    bool Hashtable::IsSynchronized()
      {
      return false;
      }

    bool Hashtable::IsFixedSize()
      {
      return false;
      }

    bool Hashtable::IsReadOnly()
      {
      return false;
      }

    void Hashtable::Add(Object* key, Object* value)
      {
      PutImpl(key, value, false);
      }

    bool Hashtable::Contains(Object* key)
      {
      return(Find(key) >= 0);
      }

    void Hashtable::Clear()
      {
      for(int i = 0; i < _table.Length(); i++)
        {
        delete _table[i].key;
        _table[i].key = nullptr;
        delete _table[i].value;
        _table[i].value = nullptr;
        _hashes.Length(0);
        }
      _inUse = 0;
      _modificationCount++;
      }

    Object* Hashtable::Get(Object* key)
      {
      if(key == nullptr)
        throw ArgumentNullException(L"key", L"null key");

      uint32 size = (uint32)_table.Length();
      int32 h = GetHash(key) & Int32::MaxValue;
      uint32 indx = (uint32)h;
      uint32 step = (uint32) ((h >> 5)+1) % (size-1)+1;


      for(uint32 i = size; i > 0; i--)
        {
        indx %= size;
        Slot entry = _table[indx];
        int hashMix = _hashes[indx];
        Object* k = entry.key;
        if(k == nullptr)
          break;

        if(k == key || ((hashMix & Int32::MaxValue) == h
          && KeyEquals(key, k)))
          {
          return entry.value;
          }

        if((hashMix & CHAIN_MARKER) == 0)
          break;

        indx += step;
        }

      return nullptr;
      }

    void Hashtable::Remove(Object* key)
      {
      int i = Find (key);
      if (i >= 0)
        {
        int h = _hashes[i];
        h &= CHAIN_MARKER;
        _hashes[i] = h;
        _table[i].key = (h != 0) ? &Removed : nullptr;
        _table[i].value = nullptr;
        --_inUse;
        ++_modificationCount;
        }
      }

    void Hashtable::Init(int capacity, float loadFactor)
      {
      if(capacity < 0)
        throw ArgumentOutOfRangeException(L"capacity", L"negative capacity");
      if(loadFactor < 0.1 || loadFactor > 1 || Single::IsNaN(loadFactor))
        throw ArgumentOutOfRangeException(L"loadFactor", L"load factor");
      if(capacity == 0)
        capacity++;
      _loadFactor = 0.75f * loadFactor;
      double num = (double)((float)capacity / _loadFactor);
      if (num > 2147483647.0)
        throw ArgumentException(L"Size is too big");
      int num2 = (int)num;
      num2 = HashPrimeNumbers::ToPrime(num2);
      Array<Slot> slot(num2);
      IntArray hashes(num2, 0);
      SetTable(slot, hashes);
      }

    void Hashtable::AdjustThreshold()
      {
      int size = (int)_table.Length();

      _threshold = (int)(size*_loadFactor);
      if(_threshold >= size)
        _threshold = size-1;
      }
    void Hashtable::SetTable(Array<Slot>& table, IntArray& hashes)
      {
      _table = table;
      _hashes = hashes;
      AdjustThreshold();
      }

    void Hashtable::PutImpl(Object* key, Object* value, bool overwrite)
      {
      if(key == nullptr)
        throw ArgumentNullException(L"key", L"null key");

      if(_inUse >= _threshold) 
        Rehash();

      uint32 size = (uint32)_table.Length();

      int32 h = GetHash(key) & Int32::MaxValue;
      uint32 spot = (uint32)h;
      uint32 step = (uint32) ((spot>>5)+1)% (size-1)+1;
      Slot entry;

      int freeIndx = -1;
      for(uint32 i = 0; i < size; i++)
        {
        int32 indx = (int32)(spot % size);
        entry = _table[indx];
        int32 hashMix = _hashes[indx];

        if(freeIndx == -1
          && entry.key == &Removed
          && (hashMix & CHAIN_MARKER) != 0)
          freeIndx = indx;

        if(entry.key == nullptr ||
          (entry.key == &Removed
          && (hashMix & CHAIN_MARKER) == 0))
          {

          if(freeIndx == -1)
            freeIndx = indx;
          break;
          }

        if((hashMix & Int32::MaxValue) == h && KeyEquals(key, entry.key))
          {
          if (overwrite)
            {
            _table[indx].value = value;
            ++_modificationCount;
            } 
          else 
            {
            // Handle Add ():
            // An entry with the same key already exists in the Hashtable.
            String err;
            err.Format(L"Key duplication when adding: {0}", key);
            throw ArgumentException(err);
            }
          return;
          }

        if(freeIndx == -1) 
          {
          _hashes[indx] |= CHAIN_MARKER;
          }

        spot+= step;

        }

      if(freeIndx!= -1) 
        {
        _table[freeIndx].key = key;
        _table[freeIndx].value = value;
        _hashes[freeIndx] |= h;

        ++_inUse;
        ++_modificationCount;
        }

      }

    void Hashtable::Rehash()
      {
      int32 oldSize = (int32)_table.Length();

      // From the SDK docs:
      //   Hashtable is automatically increased
      //   to the smallest prime number that is larger
      //   than twice the current number of Hashtable buckets
      uint32 newSize = (uint32)HashPrimeNumbers::ToPrime((oldSize<<1)|1);


      Array<Slot> newTable(newSize);
      Array<Slot> table = _table;
      IntArray newHashes(newSize);
      IntArray hashes = _hashes;

      for(int32 i = 0; i < oldSize; i++) 
        {
        Slot s = table[i];
        if(s.key!= nullptr)
          {
          int h = hashes[i] & Int32::MaxValue;
          uint32 spot = (uint32)h;
          uint32 step = ((uint32)(h>>5)+1)% (newSize-1)+1;
          for(uint32 j = spot%newSize;;spot+= step, j = spot%newSize) 
            {
            // No check for KeyMarker.Removed here,
            // because the table is just allocated.
            if (newTable[j].key == nullptr) 
              {
              newTable[j].key = s.key;
              newTable[j].value = s.value;
              newHashes[j] |= h;
              break;
              } 
            else
              {
              newHashes[j] |= CHAIN_MARKER;
              }
            }
          }
        }

      ++_modificationCount;

      SetTable(newTable, newHashes);
      }

    int Hashtable::GetHash(Object* key)
      {
      if(_equalityComparer != nullptr)
        return _equalityComparer->GetHashCode(key);
      if(_hcpRef == nullptr)
        return key->GetHashCode ();

      return _hcpRef->GetHashCode(key);
      }

    bool Hashtable::KeyEquals(Object* item, Object* key)
      {
      if(key == &Removed)
        return false;
      if(_equalityComparer != nullptr)
        return _equalityComparer->Equals(item, key);
      if(_comparerRef == nullptr)
        return item->Equals(key);

      return _comparerRef->Compare(item, key) == 0;
      }

    int Hashtable::Find(Object* key)
      {
      if(key == nullptr)
        throw ArgumentNullException(L"key", L"null key");

      uint32 size = (uint32)_table.Length();
      int h = GetHash(key) & Int32::MaxValue;
      uint32 indx = (uint32)h;
      uint32 step = (uint32)((h >> 5)+1) % (size-1)+1;


      for(uint32 i = size; i > 0; i--)
        {
        indx %= size;
        Slot entry = _table[indx];
        int hashMix = _hashes[indx];
        Object* k = entry.key;
        if(k == nullptr)
          break;

        if(k == key || ((hashMix & Int32::MaxValue) == h
          && KeyEquals(key, k)))
          {
          return (int)indx;
          }

        if((hashMix & CHAIN_MARKER) == 0)
          break;

        indx += step;
        }
      return -1;
      }
    }
  }
