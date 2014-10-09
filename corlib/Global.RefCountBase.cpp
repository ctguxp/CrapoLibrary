#include "pch.h"
#include "Global.RefCountBase.h"

namespace Global
  {
  RefCountBase::RefCountBase()
    :_uses(1)
    ,_weak(1)
    {
    }
  RefCountBase::~RefCountBase()
    {
    }
  void RefCountBase::DecrementRef()
    {
    if(::InterlockedDecrement(&_uses) == 0)
      {
      Delete();
      DecrementWeakRef();
      }
    }
  void RefCountBase::DecrementWeakRef()
    {
    if(::InterlockedDecrement(&_weak) == 0)
      {
      DeleteSelf();
      }
    }
  void RefCountBase::IncrementRef()
    {
    ::InterlockedIncrement(&_uses);
    }
  }
