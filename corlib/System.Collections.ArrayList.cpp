#include "pch.h"
#include "System.Collections.ArrayList.h"

namespace System
  {
  namespace Collections
    {
    ArrayList::ArrayList()
      :_size(0)
      ,_version(0)
      ,_items()
      {
      }
    ArrayList::~ArrayList()
      {
      }
    int ArrayList::Count()
      {
      return _size;
      }
    bool ArrayList::IsSynchronized()
      {
      return false;
      }
    void ArrayList::Clear()
      {
      _items.Length(0);
      _size = 0;
      _version++;
      }
    }
  }
