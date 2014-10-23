#include "pch.h"
#include "System.Collections.Hashtable.h"
#include "System.Exception.h"
#include "System.Single.h"
#include "System.Collections.HashPrimeNumbers.h"
#include "System.Collections.DictionaryEntry.h"
#include "System.Array.hpp"

using namespace Global;

namespace System
  {
  namespace Collections
    {
    GCObject Hashtable::Removed(new Hashtable::KeyMarker());

    Hashtable::Enumerator::Enumerator(Hashtable& host, EnumeratorMode mode)
      :_pos(-1)
      ,_stamp(host._modificationCount)
      ,_size((int32)host._table.Length())
      ,_mode(mode)
      ,_host(host)
      ,_currentKey()
      ,_currentValue()
      ,_currentDictionary()
      {
      }

    Hashtable::Enumerator::~Enumerator()
      {
      } 

    GCObject& Hashtable::Enumerator::Current()
      {
      if(_currentKey.Get() == nullptr)
        //throw InvalidOperationException();
          throw SystemException(L"InvalidOperation");

      switch(_mode) 
        {
        case EnumeratorMode::KEY_MODE:
          {
          return _currentKey;
          }
        case EnumeratorMode::VALUE_MODE:
          {
          return _currentValue;
          }
        case EnumeratorMode::ENTRY_MODE:
          {
          return _currentDictionary;
          }
        }
      throw Exception(L"should never happen");
      }

    void Hashtable::Enumerator::Reset()
      {
      FailFast();

      _pos = -1;
      _currentKey.Reset();
      _currentValue.Reset();
      _currentDictionary.Reset();
      }

    bool Hashtable::Enumerator::MoveNext()
      {
      FailFast ();

      if(_pos < _size)
        {
        while(++_pos < _size)
          {
          Slot entry = _host._table[_pos];

          if(entry.key.Get() != nullptr && entry.key.Get() != Removed.Get())
            {
            _currentKey = entry.key;
            _currentValue = entry.value;
            _currentDictionary.Reset(new DictionaryEntry(_currentKey, _currentValue));
            return true;
            }
          }
        }

      _currentKey.Reset();
      _currentValue.Reset();
      _currentDictionary.Reset();
      return false;
      }

    GCObject Hashtable::Enumerator::Key()
      {
      if(_currentKey.Get() == nullptr)
        //throw new InvalidOperationException ();
          throw SystemException(L"InvalidOperationException");
      FailFast();
      return _currentKey;
      }

    GCObject Hashtable::Enumerator::Value()
      {
      if(_currentKey.Get() == nullptr)
        //throw new InvalidOperationException ();
          throw SystemException(L"InvalidOperationException");
      FailFast();
      return _currentValue;
      }

    void Hashtable::Enumerator::FailFast()
      {
      if(_host._modificationCount != _stamp)
        {
        //throw new InvalidOperationException (xstr);
        throw SystemException(L"InvalidOperationException Hashtable.Enumerator: snapshot out of sync.");
        }
      }

    Hashtable::Hashtable(sizet capacity, float loadFactor, IHashCodeProvider* hcp, IComparer* comparer)
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
      for(sizet i = 0; i < _table.Length(); ++i)
        {
        if(_table[i].key.Get() != nullptr)
          _table[i].key.Reset();
        if(_table[i].value.Get() != nullptr)
          _table[i].value.Reset();
        }
      }

    int32 Hashtable::Count()
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
      GCObject k(key);
      GCObject v(value);
      PutImpl(k, v, false);
      }

    bool Hashtable::Contains(Object* key)
      {
      return(Find(key) >= 0);
      }

    bool Hashtable::ContainsValue(Object* value)
      {
      int32 size = (int32)_table.Length();
      if (value == nullptr) 
        {
        for(int32 i = 0; i < size; i++) 
          {
          Slot entry = _table[i];
          if (entry.key.Get() != nullptr && entry.key.Get() != Removed.Get()
            && entry.value.Get() == nullptr) 
            {
            return true;
            }
          }
        } 
      else 
        { 
        for(int32 i = 0; i < size; i++) 
          {
          Slot entry = _table [i];
          if (entry.key.Get() != nullptr && entry.key.Get() != Removed.Get()
            && value->Equals(entry.value.Get())) 
            {
            return true;
            }
          }
        }
      return false;
      }

    void Hashtable::Clear()
      {
      for(sizet i = 0; i < _table.Length(); i++)
        {
        _table[i].key.Reset();
        _table[i].value.Reset();
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
        GCObject k = entry.key;
        if(k.Get() == nullptr)
          break;

        if(k.Get()->Equals(key) || ((hashMix & Int32::MaxValue) == h
          && KeyEquals(key, k)))
          {
          return entry.value.Get();
          }

        if((hashMix & CHAIN_MARKER) == 0)
          break;

        indx += step;
        }

      return nullptr;
      }

    void Hashtable::Remove(Object* key)
      {
      int i = Find(key);
      if (i >= 0)
        {
        int h = _hashes[i];
        h &= CHAIN_MARKER;
        _hashes[i] = h;
        if(h != 0)
          _table[i].key = Removed;
        else
          _table[i].key.Reset();

        _table[i].value.Reset();
        --_inUse;
        ++_modificationCount;
        }
      }

    IDictionaryEnumerator* Hashtable::GetEnumerator()
      {
      return new Enumerator(*this, EnumeratorMode::ENTRY_MODE);
      }

    void Hashtable::Init(sizet capacity, float loadFactor)
      {
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

    void Hashtable::PutImpl(GCObject& key, GCObject& value, bool overwrite)
      {
      if(key.Get() == nullptr)
        throw ArgumentNullException(L"key", L"null key");

      if(_inUse >= _threshold) 
        Rehash();

      uint32 size = (uint32)_table.Length();

      int32 h = GetHash(key.Get()) & Int32::MaxValue;
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
          && entry.key.Get() == Removed.Get()
          && (hashMix & CHAIN_MARKER) != 0)
          freeIndx = indx;

        if(entry.key.Get() == nullptr ||
          (entry.key.Get() == Removed.Get()
          && (hashMix & CHAIN_MARKER) == 0))
          {

          if(freeIndx == -1)
            freeIndx = indx;
          break;
          }

        if((hashMix & Int32::MaxValue) == h && KeyEquals(key.Get(), entry.key))
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
            err.Format(L"Key duplication when adding: {0}", key.Get());
            throw ArgumentException(err);
            }
          return;
          }

        if(freeIndx == -1) 
          {
          _hashes[indx] = _hashes[indx] | CHAIN_MARKER;
          }

        spot+= step;

        }

      if(freeIndx!= -1) 
        {
        _table[freeIndx].key = key;
        _table[freeIndx].value = value;
        _hashes[freeIndx] = _hashes[freeIndx] | h;

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
      IntArray newHashes(newSize);
      for(sizet z = 0; z < newHashes.Length(); ++z)
        newHashes[z] = 0;

      for(int32 i = 0; i < oldSize; i++) 
        {
        Slot s = _table[i];
        if(s.key.Get() != nullptr)
          {
          int32 h = _hashes[i] & Int32::MaxValue;
          uint32 spot = (uint32)h;
          uint32 step = ((uint32)(h>>5)+1)% (newSize-1)+1;
          for(uint32 j = spot%newSize;;spot+= step, j = spot%newSize) 
            {
            // No check for KeyMarker.Removed here,
            // because the table is just allocated.
            if (newTable[j].key.Get() == nullptr) 
              {
              newTable[j].key = s.key;
              newTable[j].value = s.value;
              newHashes[j] = newHashes[j] | h;
              break;
              } 
            else
              {
              newHashes[j] = newHashes[j] | CHAIN_MARKER;
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
        return key->GetHashCode();

      return _hcpRef->GetHashCode(key);
      }

    bool Hashtable::KeyEquals(Object* item, GCObject& key)
      {
      if(key.Get() == Removed.Get())
        return false;
      if(_equalityComparer != nullptr)
        return _equalityComparer->Equals(item, key.Get());
      if(_comparerRef == nullptr)
        return item->Equals(key.Get());

      return _comparerRef->Compare(item, key.Get()) == 0;
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
        GCObject k = entry.key;
        if(k.Get() == nullptr)
          break;

        if(k.Get()->Equals(key) || ((hashMix & Int32::MaxValue) == h
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
