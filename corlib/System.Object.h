// Perch Lake Computer System

#pragma once
#include "System.Array.h"
#include "System.Ownership.h"
#include "Global.Memory.h"

using namespace Global;

namespace System
  {
  class String;
  class CRAPOCOREDLL_API Object
    {
    public:
      Object();
      virtual ~Object();
      static bool IsInstance(const Object&, const Object&);
      static bool ReferenceEquals(const Object&, const Object&);
      static bool Equals(Object*, Object*);
      virtual String ToString();
      virtual uint32 GetHashCode();
      virtual bool Equals(Object* obj);
    };

  typedef SharedPtr<Object> GCObject;

  class CRAPOCOREDLL_API NullObject : public Object
    {
    private:
      static NullObject _instance;
    private:
      NullObject();
    public:
      virtual uint32 NullObject::GetHashCode() override;
      static NullObject& Instance();
    };

  class CRAPOCOREDLL_API ObjectArray : public Ownership
    {
    public:
      ObjectArray(uint32 len = 0);
      ~ObjectArray();
      /* const Object& operator [] (uint32) const;*/
      Object& operator [] (uint32);
      bool IsNull();
      void Add(uint32, Object&);
      uint32 Length();
    private:
      //#pragma warning(disable:4251)
      Array<Object*> _array;
      //#pragma warning(default:4251)
    };

  inline bool operator == (Object& left, Object& right)
    {
    return left.Equals(&right);
    }

  inline bool operator == (GCObject& left, GCObject& right)
    {
    return left->Equals(right.Get());
    }

  }

