#pragma once

namespace Global
  {
  class CRAPOCOREDLL_API SharedPtrCountedBase
    {
    private:
      long _useCount;
      long _weakCount;
    public:
      SharedPtrCountedBase();
      virtual ~SharedPtrCountedBase();
      void AddRefCopy();
      virtual void Destroy();
      virtual void Dispose() = 0;
      void Release();
      void WeakRelease();
    private:
      SharedPtrCountedBase(SharedPtrCountedBase const&); 
      SharedPtrCountedBase& operator = (SharedPtrCountedBase const&);
    };

  template<class T>
  class SharedPtrCountedImpl : public SharedPtrCountedBase
    {
    private:
      T* _px;
    public:
      explicit SharedPtrCountedImpl(T* px)
        :_px(px)
        {
        }
      virtual void Dispose() override
        {
        delete _px;
        }
    private:
      SharedPtrCountedImpl(SharedPtrCountedImpl const&); 
      SharedPtrCountedImpl& operator = (SharedPtrCountedImpl const&);
    };
  }
