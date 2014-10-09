#pragma once

namespace Global
{
class RefCountBase
  {
  public:
    virtual ~RefCountBase();
    void DecrementRef();
    void DecrementWeakRef();
    void IncrementRef();
  protected:
    RefCountBase();
  private:
    virtual void Delete() = 0;
    virtual void DeleteSelf() = 0;
  private:
    uint32 _uses;
    uint32 _weak;
  };
}
