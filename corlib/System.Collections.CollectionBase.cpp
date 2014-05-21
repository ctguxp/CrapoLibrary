#include "pch.h"
#include "System.Collections.CollectionBase.h"

namespace System
  {
  namespace Collections
    {
    CollectionBase::CollectionBase()
      :_list()
      {
      }
    CollectionBase::CollectionBase(sizet capacity)
      :_list(capacity)
      {
      }
    CollectionBase::~CollectionBase()
      {
      }
    }
  }
