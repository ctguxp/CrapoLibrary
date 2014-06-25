// Perch Lake Computer System

#pragma once

namespace System
{
class CRAPOCOREDLL_API Ownership
  {
  private:
    bool _isOwner;
  protected:
    Ownership();
    Ownership(Ownership const& owner);
    Ownership& operator=(Ownership const& owner);
  public:
    ~Ownership();
    void AssertOwnership();
    bool IsOwner();
    void RescindOwnership();
  };
}
