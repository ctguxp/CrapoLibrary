#include "pch.h"
#include "Global.SharedPtrCountedBase.h"

namespace Global
  {
  SharedPtrCountedBase::SharedPtrCountedBase()
    :_useCount(1)
    ,_weakCount(1)
    {
    }
  SharedPtrCountedBase::~SharedPtrCountedBase()
    {
    }
  void SharedPtrCountedBase::AddRefCopy()
    {
    ::InterlockedIncrement(&_useCount);
    }
  void SharedPtrCountedBase::Destroy()
    {
    delete this;
    }
  void SharedPtrCountedBase::Release()
    {
    if(::InterlockedDecrement(&_useCount) == 0 )
      {
      Dispose();
      WeakRelease();
      }
    }
  void SharedPtrCountedBase::WeakRelease()
    {
    if(::InterlockedDecrement(&_weakCount) == 0 )
      {
      Destroy();
      }
    }
  }
