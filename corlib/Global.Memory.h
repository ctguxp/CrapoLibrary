#pragma once
#include "Global.Utility.h"
#include "Global.SharedCount.h"

namespace Global
  {
  template<class T>
  class SharedPtr
    {
    typedef T e_type;
    typedef SharedPtr<T> c_type;
    private:
      e_type*     _px;
      SharedCount _pn;
    public:
      SharedPtr()
        :_px(nullptr)
        ,_pn()
        {
        }
      SharedPtr(SharedPtr const& sp)
        :_px(sp._px)
        ,_pn(sp._pn)
        {
        }
      template<class U>
      explicit SharedPtr(U* px)
        :_px(px)
        ,_pn()
        {
        ConstructSharedPtr(this, px, _pn);
        }
      template<class U>
      SharedPtr(SharedPtr<U> const& other)
        :_px(other._px)
        ,_pn(other._pn)
        {
        }
      ~SharedPtr()
        {
        }
      SharedPtr& operator= (SharedPtr const& sp)
        {
        c_type(sp).Swap(*this);
        return *this;
        }
      e_type& operator* ()
        {
        assert(_px != nullptr);
        return *_px;
        }
      e_type* operator-> () const
        {
        assert(_px != nullptr);
        return _px;
        }
      e_type* Get() const
        {
        return _px;
        }
      void Reset()
        {
        c_type().Swap(*this);
        }
      template<class U>
      void Reset(U* p)
        {
        assert(p == nullptr || p != _px );
        c_type(p).Swap(*this);
        }
      void Swap(SharedPtr& other)
        {
        Global::Swap(_px, other._px);
        _pn.Swap(other._pn);
        }
    private:
      template<class T>
      friend class SharedPtr;
    };

  template<class T, class U>
  inline void ConstructSharedPtr(SharedPtr<T>* /*ptr*/, U* p, SharedCount& sc)
    {
    SharedCount(p).Swap(sc);
    }
  }
