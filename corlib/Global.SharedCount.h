#pragma once
#include "Global.SharedPtrCountedBase.h"

namespace Global
  {
  class SharedCount
    {
    private:
      SharedPtrCountedBase* _pi;
    public:
      SharedCount();
      SharedCount(SharedCount const& sc);
      template <class T> explicit
        SharedCount(T* p)
        :_pi(nullptr)
        {
        try
          {
          _pi = new SharedPtrCountedImpl<T>(p);
          }
        catch(...)
          {
          }
        }

      ~SharedCount();
      void Swap(SharedCount& r);
    };
  }
