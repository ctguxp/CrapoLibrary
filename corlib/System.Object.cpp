// Perch Lake Computer System

#include "pch.h"
#include "System.Object.h"
#include "System.String.h"
#include "System.Array.hpp"

namespace System
  {

  NullObject NullObject::_instance;

  // Default constructor
  Object::Object()
    {
    }
  // Destructor
  Object::~Object()
    {
    }
  // Is same instance type
  bool Object::IsInstance(const Object& o1, const Object& o2)
    {
    return (typeid(o1) == typeid(o2));
    }
  bool Object::ReferenceEquals(const Object& o1, const Object& o2)
    {
    return (&o1 == &o2);
    }
  uint32 Object::GetHashCode()
    {
    return (uint32)typeid(*this).hash_code();
    }
  String Object::ToString()
    {
    const char* temp = typeid(*this).name();
    uint32 len = (uint32)strlen(temp);
    CharArray buffer(len + 1);
    ::MultiByteToWideChar(20127, 0, temp, len, buffer.ToPtr(), len + 1);
    buffer[len] = L'\0';
    return String(buffer.ToConstPtr());
    }

  bool Object::Equals(Object* obj)
    {
    return this == obj;
    }

  NullObject::NullObject()
    {
    }

  uint32 NullObject::GetHashCode()
    {
    return 0;
    }

  NullObject& NullObject::Instance()
    {
    return _instance;
    }

  ObjectArray::ObjectArray(uint32 len)
    :_array(len)
    {
    }

  ObjectArray::~ObjectArray()
    {
    if(IsOwner())
      {
      for(uint32 i = 0; i < _array.Length(); ++i)
        {
        delete _array[i];
        _array[i] = nullptr;
        }
      }
    }

  void ObjectArray::Add(uint32 idx, Object& obj)
    {
    _array[idx] = &obj;
    }

  const Object& ObjectArray::operator [] (uint32 idx) const
    {
    return *_array[idx];
    }

  Object& ObjectArray::operator [] (uint32 idx)
    {
    return *_array[idx];
    }

  bool ObjectArray::IsNull()
    {
    return _array.IsNull();
    }

  uint32 ObjectArray::Length()
    {
    return (uint32)_array.Length();
    }
  }
