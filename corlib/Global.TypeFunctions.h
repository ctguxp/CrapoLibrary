#pragma once
#include "System.Char.h"
#include "System.String.h"
#include "System.Array.h"

using namespace System;

inline bool IsComparable(wchar_t c)
  {
  Char test(c);
  IComparableT<Char>* p = dynamic_cast<IComparableT<Char>*>(&test);
  IComparable* a = dynamic_cast<IComparable*>(&test);
  if(p == nullptr || a == nullptr)
    return false;

  return true;
  }

inline bool IsComparable(String& s)
  {
  IComparableT<String>* p = dynamic_cast<IComparableT<String>*>(&s);
  IComparable* a = dynamic_cast<IComparable*>(&s);
  if(p == nullptr || a == nullptr)
    return false;
  return true;
  }

inline bool IsComparable(GCObject& obj)
  {
  IComparable* a = dynamic_cast<IComparable*>(obj.Get());
  if(a == nullptr)
    return false;
  return true;
  }

inline GCObject GetNewObject(wchar_t c)
  {
  return GCObject(new Char(c));
  }

inline GCObject GetNewObject(String& s)
  {
  return GCObject(new String(s));
  }

inline GCObject GetNewObject(GCObject& s)
  {
  return s;
  }

inline Char* GetCharType(Object* obj)
  {
  return dynamic_cast<Char*>(obj);
  }

inline String* GetStringType(Object* obj)
  {
  return dynamic_cast<String*>(obj);
  }

inline void SetCharArrayValue(wchar_t* c, Char& value)
  {
  (*c) = (wchar_t)value;
  }

inline void SetStringArray(String* s, String& value)
  {
  (*s) = value;
  }