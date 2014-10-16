#include "pch.h"
#include "Global.SharedCount.h"

namespace Global
  {
  SharedCount::SharedCount()
    :_pi(nullptr)
    {
    }
  SharedCount::SharedCount(SharedCount const& sc)
      :_pi(sc._pi)
      {
      if(_pi != nullptr)
        _pi->AddRefCopy();
      }
  SharedCount::~SharedCount()
    {
    if(_pi != nullptr)
      _pi->Release();
    }
  void SharedCount::Swap(SharedCount& r)
    {
    SharedPtrCountedBase* tmp = r._pi;
    r._pi = _pi;
    _pi = tmp;
    }
  }
