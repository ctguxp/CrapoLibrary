#include "pch.h"
#include "System.Array.hpp"
#include "System.Collections.Queue.h"
#include "System.NotImplementedException.h"
#include "System.Exception.h"

namespace System
  {
  namespace Collections
    {
    Queue::Queue(int32 capacity, float growFactor)
      :_head(0)
      ,_size(0)
      ,_tail(0)
      ,_growFactor(0)
      ,_version(0)
      ,_array()
      {
      if(capacity < 0)
        throw ArgumentOutOfRangeException(L"capacity", L"Needs a non-negative number");
      if(!(growFactor >= 1.0F && growFactor <= 10.0F))
        throw ArgumentOutOfRangeException(L"growFactor", L"Queue growth factor must be between 1.0 and 10.0, inclusive");

      _array.Length(capacity);
      _growFactor = (int32)(growFactor * 100);
      }

    Queue::Queue(const Queue& queue)
      :_head(queue._head)
      ,_size(queue._size)
      ,_tail(queue._tail)
      ,_growFactor(queue._growFactor)
      ,_version(queue._version)
      ,_array(queue._array)
      {
      }

    Queue::~Queue()
      {
      }

    void Queue::Clear() 
      {
      _version++;
      _head = 0;
      _size = 0;
      _tail = 0;
      for(int32 length = (int32)_array.Length() - 1; length >= 0; length--)
        _array[length].Reset();
      }

    int32 Queue::Count()
      {
      return _size;
      }

    bool Queue::Contains(Object* obj)
      {
      int32 tail = _head + _size;
      if(obj == nullptr) 
        {
        for(int32 i = _head; i < tail; i++) 
          {
          if(_array[i % (int32)_array.Length()].Get() == nullptr) 
            return true;
          }
        } 
      else 
        {
        for(int32 i = _head; i < tail; i++) 
          {
          if(obj->Equals(_array[i % (int32)_array.Length()].Get()))
            return true;
          }
        }
      return false;
      }

    bool Queue::IsSynchronized()
      {
      return false;
      }

    IEnumerator* Queue::GetEnumerator()
      {
      throw NotImplementedException();
      }

    void Queue::Enqueue(GCObject& obj) 
      {
      _version++;
      if(_size == (int32)_array.Length()) 
        Grow();
      _array[_tail] = obj;
      _tail = (_tail+1) % (int32)_array.Length();
      _size++;
      }

    GCObject Queue::Dequeue()
      {
      _version++;
      if(_size < 1)
        //throw InvalidOperationException();
          throw SystemException(L"Invalid Operation");
      GCObject result = _array[_head];
      _array[_head].Reset();
      _head = (_head + 1) % (int32)_array.Length();
      _size--;
      return result;
      }

    void Queue::Grow () 
      {
      int32 newCapacity = ((int32)_array.Length() * _growFactor) / 100;
      if(newCapacity < (int32)_array.Length() + 1)
        newCapacity = (int32)_array.Length() + 1;
      //object[] newContents = new object[newCapacity];
      //CopyTo(newContents, 0);
      //_array = newContents;
      _array.Length(newCapacity);
      _head = 0;
      _tail = _head + _size;
      }
    }
  }
