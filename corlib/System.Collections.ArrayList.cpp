#include "pch.h"
#include "System.Collections.ArrayList.h"
#include "System.Array.hpp"

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
    ArrayList::ArrayList(sizet capacity)
      :_size(capacity != 0 ? capacity : 4)
      ,_version(0)
      ,_items(_size)
      {
      for(sizet i = 0; i < _items.Length(); ++i)
        _items[i] = nullptr;
      }
    ArrayList::~ArrayList()
      {
      Free();
      }
    size_t ArrayList::Count()
      {
      return _size;
      }
    bool ArrayList::IsSynchronized()
      {
      return false;
      }
    void ArrayList::Clear()
      {
      Free();
      _items.Length(0);
      _size = 0;
      _version++;
      }
    void ArrayList::Free()
      {
      for(sizet i = 0; i < _items.Length(); ++i)
        {
        if(_items[i] != nullptr)
          {
          delete _items[i];
          _items[i] = nullptr;
          }
        }
      }
    }
  }
