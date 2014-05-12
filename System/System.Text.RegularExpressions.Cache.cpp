#include "pch.h"
#include "System.Text.RegularExpressions.Cache.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      MRUList::Node::Node(Object* value)
        :_value(value)
        {
        }

      MRUList::MRUList()
        :_head(nullptr)
        ,_tail(nullptr)
        {
        }

      void MRUList::Use(Object* o)
        {
        Node* node = nullptr;

        if(_head == nullptr) 
          {
          node = new Node(o);
          _head = _tail = node;
          return;
          }

        node = _head;
        while(node != nullptr && !o->Equals(node->_value))
          node = node->_previous;

        if(node == nullptr)
          node = new Node(o);
        else 
          {
          if(node == _head)
            return;

          if(node == _tail)
            _tail = node->_next;
          else
            node->_previous->_next = node->_next;

          node->_next->_previous = node->_previous;
          }

        _head->_next = node;
        node->_previous = _head;
        node->_next = nullptr;
        _head = node;
        }

      Object* MRUList::Evict()
        {
        if(_tail == nullptr)
          return nullptr;

        Object* o = _tail->_value;
        _tail = _tail->_next;

        if(_tail == nullptr)
          _head = nullptr;
        else
          _tail->_previous = nullptr;

        return o;
        }

      FactoryCache::FactoryCache(int capacity)
        :_capacity(capacity)
        ,_factories(capacity)
        ,_mruList()
        {
        }

      FactoryCache::~FactoryCache()
        {
        }

      void FactoryCache::Add(String pattern, RegexOptions options, IMachineFactory* factory)
        {
        //lock (this) {
        Key* k = new Key(pattern, options);
        Cleanup();
        _factories.Add(k, factory);
        _mruList.Use(k);
        //}
        }

      IMachineFactory* FactoryCache::Lookup(String pattern, RegexOptions options)
        {
        // TODO lock (this) {
        Key* k = new Key(pattern, options);
        if(_factories.Contains(k))
          {
          _mruList.Use(k);
          return (IMachineFactory*)_factories.Get(k);
          }
        //}

        return nullptr;
        }

      // lock must be held by the caller
      void FactoryCache::Cleanup()
        {
        while(_factories.Count() >= _capacity && _capacity > 0)
          {
          Object* victim = _mruList.Evict();
          if(victim != nullptr)
            _factories.Remove((Key*)victim);
          }
        }

      FactoryCache::Key::Key(String pattern, RegexOptions options)
        :_pattern(pattern)
        ,_options(options)
        {
        }

      uint32 FactoryCache::Key::GetHashCode()
        {
        return _pattern.GetHashCode() ^ (uintptr)_options;
        }
      }
    }
  }
