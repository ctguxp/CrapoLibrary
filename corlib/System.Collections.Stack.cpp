#include "pch.h"
#include "System.Array.hpp"
#include "System.Exception.h"
#include "System.Math.h"
#include "System.Collections.Stack.h"

namespace System
  {
  namespace Collections
    {
    Stack::Stack()
      :_capacity(default_capacity)
      ,_count(0)
      ,_current(-1)
      ,_modCount(0)
      ,_contents(_capacity)
      {
      sizet s = _contents.Length();
      for(sizet i = 0; i < s; ++i)
        _contents[i] = nullptr;  
      }
    Stack::Stack(sizet initialCapacity)
      :_capacity(initialCapacity)
      ,_count(0)
      ,_current(-1)
      ,_modCount(0)
      ,_contents(_capacity)
      {
      sizet s = _contents.Length();
      for(sizet i = 0; i < s; ++i)
        _contents[i] = nullptr;  
      }
    Stack::~Stack()
      {
      for(sizet i = 0; i < _count; ++i)
        {
        if(_contents[i] != nullptr)
          {
          delete _contents[i];
          _contents[i] = nullptr;
          }
        }
      }
    sizet Stack::Count()
      {
      return _count;
      }
    bool Stack::IsSynchronized()
      {
      return false;
      }
    bool Stack::Contains(Object* obj)
      {
      if(_count == 0)
        return false;

      if(obj == nullptr)
        {
        for(sizet i = 0; i < _count; i++)
          {
          if(_contents[i] == nullptr)
            return true; 
          }
        } 
      else 
        {
        for(sizet i = 0; i < _count; i++) 
          {
          if(obj->Equals(_contents[i]))
            return true; 
          }
        }

      return false;
      }
    Object* Stack::Pop()
      {
      if(_current == -1)
        {
        //throw InvalidOperationException();
        throw SystemException(L"Invalid Operation");
        } 
      else 
        {
        _modCount++;

        Object* ret = _contents[_current];
        _contents[_current] = nullptr;

        _count--;
        _current--;

        // if we're down to capacity/4, go back to a 
        // lower array size.  this should keep us from 
        // sucking down huge amounts of memory when 
        // putting large numbers of items in the Stack.
        // if we're lower than 16, don't bother, since 
        // it will be more trouble than it's worth.
        if(_count <= (_capacity/4) && _count > 16) 
          {
          Resize(_capacity/2);
          }

        return ret;
        }
      }
    void Stack::Push(Object* obj)
      {
      _modCount++;

      if(_capacity == _count)
        {
        Resize(_capacity * 2);
        }

      _count++;
      _current++;

      _contents[_current] = obj;
      }
    void Stack::Resize(sizet ncapacity)
      {
      ncapacity = Math::Max((uintptr)ncapacity, (uintptr)default_capacity);
      Array<Object*> ncontents(ncapacity);
      sizet s = ncontents.Length();
      for(sizet i = 0; i < s; ++i)
        ncontents[i] = nullptr; 

      Array<Object*>::Copy(_contents, 0, ncontents, 0, _count);		
      _capacity = ncapacity;
      _contents = ncontents;
      }
    }
  }
