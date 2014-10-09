#pragma once
#include "Global.RefCountBase.h"

namespace Global
{

template<class T>
class RefCount : public RefCountBase
  {
  public:
    RefCount(T* ptr)
      :RefCountBase()
      ,_ptr(ptr)
      {
      }
  private:
    virtual void Delete() override
      {
      delete _ptr;
      _ptr = nullptr;
      }
    virtual void DeleteSelf() override
      {
      delete this;
      }
  private:
    T* _ptr;
  };

template<class T>
class PtrBase
  {
  public:
    PtrBase()
      :_ptr(nullptr)
      ,_ref(nullptr)
      {
      }
    virtual ~PtrBase()
      {
      _ptr = nullptr;
      }
    void DecrementRef()
      {
      if(_ref != nullptr)
        _ref->DecrementRef();
      _ref = nullptr;
      }
    T* GetPtr() const
      {
      return _ptr;
      }
    void Reset(T* ptr, RefCountBase* ref)
      {
      if(_ref != nullptr)
        _ref->DecrementRef();
      _ptr = ptr;
      _ref = ref;
      }
    void ResetOther(const PtrBase<T>& other)
      {
      ResetAndTakeOther(other._ptr, other._ref);
      }
    void ResetAndTakeOther(T* otherPtr, RefCountBase* otherRep)
      {
      if(otherRep)
        otherRep->IncrementRef();
      Reset(otherPtr, otherRep);
      }
  private:
    T*          _ptr;
    RefCountBase* _ref;
  };

template<class T>
class SharedPtr : public PtrBase<T>
  {
  public:
    SharedPtr(T* ptr = nullptr)
      {
      ResetPtr(ptr);
      }
    SharedPtr(const SharedPtr<T>& other)
      {
      ResetOther(other);
      }
    ~SharedPtr()
      {
      DecrementRef();
      }
    T* operator->() const
		  {
		  return Get();
		  }
     T* Get() const
      {
      return this->GetPtr();
      }
    void ResetPtr(T* ptr, RefCountBase* ref)
      {
      Reset(ptr, ref);
      }
  private:
    void ResetPtr(T* ptr)
      {
      try
        {
        ResetPtr(ptr, new RefCount<T>(ptr));
        }
      catch(...)
        {
        delete ptr;
        throw;
        }
      }
  };
}
