#include "pch.h"
#include "System.Ownership.h"

namespace System
  {
  Ownership::Ownership(bool isOwner)
    :_isOwner(isOwner)
    {
    }
  Ownership::Ownership(Ownership const& owner)
    :_isOwner(owner._isOwner)
    {
    }
  Ownership::~Ownership()
    {
    }
  Ownership& Ownership::operator=(Ownership const& owner)
    {
    if(this == &owner)
      return *this;

    _isOwner = owner._isOwner;
    return *this;
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
