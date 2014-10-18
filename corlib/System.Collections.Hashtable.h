#pragma once
#include "System.Collections.IDictonary.h"
#include "System.Collections.IHashCodeProvider.h"
#include "System.Collections.IComparer.h"
#include "System.Collections.IEqualityComparer.h"
#include "System.Int32.h"
#include "Global.Memory.h"

using namespace Global;

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
          :key()
          ,value()
          {
          }
          GCObject key;
          GCObject value;
          };
       public:
        class CRAPOCOREDLL_API KeyMarker : public Object
          {
          public:
            KeyMarker(){}
          };
        protected:
         class Enumerator : public IDictionaryEnumerator
          {
          private:
            int32          _pos;
            int32          _stamp;
            int32          _size;
            EnumeratorMode _mode;
            Hashtable&     _host;
            GCObject        _currentKey;
            GCObject        _currentValue;
          private:
            Enumerator& operator= (Enumerator const&);
            Enumerator(Enumerator const&);
          public:
            Enumerator(Hashtable& host, EnumeratorMode mode);
            virtual ~Enumerator();
            // From IEnumerator
            virtual GCObject Current() override;
            virtual void Reset() override;
            virtual bool MoveNext() override;
            virtual GCObject Key() override;
            virtual GCObject Value() override;
          private:
            void FailFast();
          };
      protected:
        static GCObject Removed;
      public:
        Hashtable(sizet capacity = 0, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        Hashtable(IDictionary* d, float loadFactor = 1, IHashCodeProvider* hcp = nullptr, IComparer* comparer = nullptr);
        virtual ~Hashtable();
        virtual IDictionaryEnumerator* GetEnumerator() override;
        Object* Get(Object* key);
        // From ICollection
        virtual int32 Count() override;
        virtual bool IsSynchronized() override;
        // From IDictionary
        virtual bool IsFixedSize() override;
        virtual bool IsReadOnly() override;
        virtual void Add(Object* key, Object* value) override;
        virtual bool Contains(Object* key) override;
        virtual void Clear() override;
        virtual void Remove(Object* key) override;
      protected:
        virtual int GetHash(Object* key);
        virtual bool KeyEquals(Object* item, GCObject& key);
      private:
        void Init(sizet, float);
        void AdjustThreshold();
        int Find(Object* key);
        void Rehash();
        void SetTable(Array<Slot>& table, IntArray& hashes);
        void PutImpl(GCObject& key, GCObject& value, bool overwrite);
      private:
        int32              _inUse;
        int32              _modificationCount;
        float              _loadFactor;
        int32              _threshold;
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
