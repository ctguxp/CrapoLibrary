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
  void Ownership::Assert()
    {
    _isOwner = true;
    }
  bool Ownership::IsOwner()
    {
    return _isOwner;
    }
  void Ownership::Rescind()
    {
    _isOwner = false;
    }
  }
