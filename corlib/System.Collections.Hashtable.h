#pragma once
#include "System.Collections.IDictonary.h"
#include "System.Collections.IHashCodeProvider.h"
#include "System.Collections.IComparer.h"
#include "System.Collections.IEqualityComparer.h"
#include "System.Int32.h"

namespace System
  {
  namespace Collections
    {
    class CRAPOCOREDLL_API Hashtable : public IDictionary
      {
      enum
        {
        CHAIN_MARKER  = ~Int32::MaxValue
        };
      protected:
        struct Slot
          {
          Slot()
          :key(nullptr)
          ,value(nullptr)
          {
          }
          Object* key;
          Object* value;
          };
        class CRAPOCOREDLL_API KeyMarker : public Object
          {
          public:
            KeyMarker(){}
          };
      protected:
        static KeyMarker Removed;
      public:
        Hashtable(sizet capacity = 0, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        Hashtable(IDictionary* d, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        virtual ~Hashtable();
        // From ICollection
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        // From IDictionary
        virtual bool IsFixedSize() override;
        virtual bool IsReadOnly() override;
        virtual void Add(Object* key, Object* value) override;
        virtual bool Contains(Object* key) override;
        virtual void Clear() override;
        Object* Get(Object* key);
        virtual void Remove(Object* key) override;
      protected:
        virtual int GetHash(Object* key);
        virtual bool KeyEquals(Object* item, Object* key);
      private:
        void Init(sizet, float);
        void AdjustThreshold();
        int Find(Object* key);
        void Rehash();
        void SetTable(Array<Slot>& table, IntArray& hashes);
        void PutImpl(Object* key, Object* value, bool overwrite);
      private:
        sizet              _inUse;
        int                _modificationCount;
        float              _loadFactor;
        int                _threshold;
        IHashCodeProvider* _hcpRef;
        IComparer*         _comparerRef;
        IEqualityComparer* _equalityComparer;
#pragma warning(disable:4251)
        Array<Slot>        _table;
        IntArray           _hashes;
#pragma warning(default:4251)
      };
    }
  }
