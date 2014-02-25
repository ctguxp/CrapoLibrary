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
  public:
    ~Ownership();
    void Assert();
    bool IsOwner();
    void Rescind();
  };
}
