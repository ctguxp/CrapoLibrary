#include "pch.h"
#include "System.Ownership.h"

namespace System
  {
  Ownership::Ownership()
    :_isOwner(true)
    {
    }
  Ownership::~Ownership()
    {
    }
  void Ownership::AssertOwnership()
    {
    _isOwner = true;
    }
  bool Ownership::IsOwner()
    {
    return _isOwner;
    }
  void Ownership::RescindOwnership()
    {
    _isOwner = false;
    }
  }
