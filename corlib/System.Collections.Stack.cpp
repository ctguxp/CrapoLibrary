#include "pch.h"
#include "System.Array.hpp"
#include "System.Exception.h"
#include "System.Math.h"
#include "System.Collections.Stack.h"

namespace System
  {
  namespace Collections
    {
    Stack::Enumerator::Enumerator(Collections::Stack& s)
      :_stack(s)
      ,_modCount(s._modCount)
      ,_current(Enumerator::Begin)
      {
      }
    Stack::Enumerator::Enumerator(const Enumerator& e)
      :_stack(e._stack)
      ,_modCount(e._modCount)
      ,_current(e._current)
      {
      }
    Stack::Enumerator& Stack::Enumerator::operator=(const Enumerator& e)
      {
      if(this == &e)
        return *this;

      _stack = e._stack;
      _modCount = e._modCount;
      _current = e._current;
      return *this;
      }
    GCObject Stack::Enumerator::Current()
      {
      if(_modCount != _stack._modCount 
        || _current == Enumerator::Begin
        || _current == Enumerator::End
        || _current > (int32)_stack._count)
        //throw new InvalidOperationException();
        throw SystemException(L"Invalid Operation");

      return _stack._contents[_current];
      }
    bool Stack::Enumerator::MoveNext()
      {
      if(_modCount != _stack._modCount)
        //throw new InvalidOperationException();
          throw SystemException(L"Invalid Operation");

      switch(_current)
        {
        case Enumerator::Begin:
          _current = _stack._current;
          return _current != -1;

        case Enumerator::End:
          return false;

        default:
          _current--; 
          return _current != -1;
        }
      }
    void Stack::Enumerator::Reset()
      {
      if(_modCount != _stack._modCount)
        //throw new InvalidOperationException();
          throw SystemException(L"Invalid Operation");

      _current = Enumerator::Begin;
      } 

    Stack::Stack()
      :_capacity(default_capacity)
      ,_count(0)
      ,_current(-1)
      ,_modCount(0)
      ,_contents(_capacity)
      {
      sizet s = _contents.Length();
      for(sizet i = 0; i < s; ++i)
        _contents[i].Reset();  
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
        _contents[i].Reset();  
      }
    Stack::~Stack()
      {
      for(sizet i = 0; i < _count; ++i)
        {
        if(_contents[i].Get() != nullptr)
          {
          _contents[i].Reset();
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
          if(_contents[i].Get() == nullptr)
            return true; 
          }
        } 
      else 
        {
        for(sizet i = 0; i < _count; i++) 
          {
          if(obj->Equals(_contents[i].Get()))
            return true; 
          }
        }

      return false;
      }
    Object& Stack::Peek()
      {
      if(_current == -1)
        {
        //throw InvalidOperationException();
        throw SystemException(L"Invalid Operation");
        } 
      else 
        {
        return (*_contents[_current]);
        }
      }
    GCObject Stack::Pop()
      {
      if(_current == -1)
        {
        //throw InvalidOperationException();
        throw SystemException(L"Invalid Operation");
        } 
      else 
        {
        _modCount++;

        GCObject ret = _contents[_current];
        _contents[_current].Reset();

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
    void Stack::Push(GCObject& obj)
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
    IEnumerator* Stack::GetEnumerator()
      {
      return new Enumerator(*this);
      }
    void Stack::Resize(sizet ncapacity)
      {
      ncapacity = Math::Max((uintptr)ncapacity, (uintptr)default_capacity);
      Array<GCObject> ncontents(ncapacity);
      sizet s = ncontents.Length();
      for(sizet i = 0; i < s; ++i)
        ncontents[i].Reset(); 

      Array<GCObject>::Copy(_contents, 0, ncontents, 0, _count);		
      _capacity = ncapacity;
      _contents = ncontents;
      }
    }
  }
