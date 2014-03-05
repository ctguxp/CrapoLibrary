// Perch Lake Computer System

#pragma once
#include "System.Ownership.h"

template<class T>
class AutoPtr : public System::Ownership
  {
  public:
    AutoPtr(T* ptr = nullptr, bool isMutable = true)
      :_ptr(ptr)
      ,_isMutable(isMutable)
      {
      }
    ~AutoPtr()
      {
      if(IsOwner())
        delete _ptr;
      }
    T* operator->() { return _ptr; }
    T* Get() { return _ptr; }
    void Set(T* ptr = nullptr)
      {
      if(_isMutable)
        {
        if(_ptr != nullptr)
          delete _ptr;
        _ptr = ptr;
        }
      }
    T* Release()
      {
      T* retval = _ptr;
      _ptr = nullptr;
      return retval;
      }
  private:
    bool _isMutable;
    T*   _ptr;
  };