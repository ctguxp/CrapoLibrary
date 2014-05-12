#pragma once
#include "System.Text.RegularExpressions.RegexOptions.h"
#include "System.Text.RegularExpressions.IMachineFactory.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class MRUList
        {
        private:
          class Node
            {
            public:
              Node(Object* value);
            public:
              Object* _value;
              Node*   _previous;
              Node*   _next;
            };
        public:
          MRUList();
          Object* Evict();
          void Use(Object* o);
        private:
          Node* _head;
          Node* _tail;
        };

      class FactoryCache
        {
        private:
          class Key : public Object
            {
            public:
              Key(String pattern, RegexOptions options);
              virtual uint32 GetHashCode() override;
            public:
              String       _pattern;
              RegexOptions _options;
            };
        public:
          FactoryCache(int /*capacity*/);
          ~FactoryCache();
          void Add(String pattern, RegexOptions options, IMachineFactory* factory);
          void Cleanup();
          IMachineFactory* Lookup(String pattern, RegexOptions options);
        private:
          int _capacity;
          Collections::Hashtable _factories;
          MRUList                _mruList;
        };
      }
    }
  }
