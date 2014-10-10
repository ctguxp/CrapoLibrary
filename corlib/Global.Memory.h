#pragma once
#include "Global.Utility.h"
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
      typedef PtrBase<T> Me;
      PtrBase()
        :_ptr(nullptr)
        ,_ref(nullptr)
        {
        }
      virtual ~PtrBase()
        {
        }
      void DecrementRef()
        {
        if(_ref != nullptr)
          {
          _ref->DecrementRef();
          _ref = nullptr;
          _ptr = nullptr;
          }
        }
      T* GetPtr() const
        {
        return _ptr;
        }
      void ResetPtrRef(T* ptr, RefCountBase* ref)
        {
        if(_ref != nullptr)
          _ref->DecrementRef();
        _ptr = ptr;
        _ref = ref;
        }
      void ResetOther(const Me& other)
        {
        ResetAndTakeOther(other._ptr, other._ref);
        }
      template<class X>
      void ResetOther(const PtrBase<X>& other)
        {
        ResetAndTakeOther(other._ptr, other._ref);
        }
      void ResetAndTakeOther(T* otherPtr, RefCountBase* otherRep)
        {
        if(otherRep)
          otherRep->IncrementRef();
        ResetPtrRef(otherPtr, otherRep);
        }
       void SwapOther(PtrBase& right)
		    {
		    Global::Swap(_ref, right._ref);
		    Global::Swap(_ptr, right._ptr);
		    }
    private:
      T*          _ptr;
      RefCountBase* _ref;
      template<class X>
      friend class PtrBase;
    };

  template<class T>
  class SharedPtr : public PtrBase<T>
    {
    public:
      typedef SharedPtr<T> Me;

      // Default constructor
      SharedPtr(T* ptr = nullptr)
        {
        ResetPtr(ptr);
        }

      // Copy Constructor SharedPtr<T>
      SharedPtr(const Me& other)
        {
        ResetOther(other);
        }

      // Copy Constructor SharedPtr<X>
      template<class X>
      SharedPtr(const SharedPtr<X>& other)
        {
        ResetOther(other);
        }

      // Destructor
      ~SharedPtr()
        {
        DecrementRef();
        }

      // Assignment Operator SharedPtr<T>
      Me& operator=(const Me& other)
        {
        SharedPtr(other).Swap(*this);
        return (*this);
        }

      template<class X>
      Me& operator=(const SharedPtr<X>& other)
        {
        SharedPtr(other).Swap(*this);
        return (*this);
        } 

      T& operator*() const
        {
        return (*Get());
        }

      T* operator->() const
        {
        return Get();
        }

      T* Get() const
        {
        return this->GetPtr();
        }

      void Reset(T* ptr = nullptr)
        {
        SharedPtr(ptr).Swap(*this);
        }

      void ResetPtr(T* ptr, RefCountBase* ref)
        {
        ResetPtrRef(ptr, ref);
        }

      void Swap(Me& other)
		    {
		    SwapOther(other);
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
