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
    AutoPtr(AutoPtr<T>& ap)
      :System::Ownership(ap)
      ,_ptr(ap._ptr)
      ,_isMutable(ap._isMutable)
      {
      if(ap.IsOwner())
        ap.RescindOwnership();
      }
    ~AutoPtr()
      {
      if(IsOwner())
        delete _ptr;
      _ptr = nullptr;
      }
    AutoPtr<T>& operator=(AutoPtr<T>& ap)
      {
      if(this == &ap)
        return *this;

      System::Ownership::operator=(ap);
      _ptr = ap._ptr;
      _isMutable = ap._isMutable;

      if(ap.IsOwner())
        ap.RescindOwnership();

      return *this;
      }
    T* operator->() { return _ptr; }
    T* Get() { return _ptr; }
    const T* Get() const { return _ptr; }
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