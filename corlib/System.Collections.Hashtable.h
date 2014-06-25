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
      enum class EnumeratorMode : int32
        {
          KEY_MODE = 0,
          VALUE_MODE,
          ENTRY_MODE
        };
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
         class Enumerator : public IDictionaryEnumerator
          {
          private:
            int32          _pos;
            int32          _stamp;
            int32          _size;
            EnumeratorMode _mode;
            Hashtable*     _host;
            Object*        _currentKey;
            Object*        _currentValue;
          public:
            Enumerator(Hashtable* host, EnumeratorMode mode);
            // From IEnumerator
            virtual GCObject Current() override;
            virtual void Reset() override;
            virtual bool MoveNext() override;
            virtual Object* Key() override;
            virtual Object* Value() override;
          private:
            void FailFast();
          };
      protected:
        static KeyMarker Removed;
      public:
        Hashtable(sizet capacity = 0, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        Hashtable(IDictionary* d, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        virtual ~Hashtable();
        virtual IDictionaryEnumerator* GetEnumerator() override;
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
