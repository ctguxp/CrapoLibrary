// Perch Lake Computer System

#pragma once

namespace win32
{
  class security_attribute
  {
  public:
    // Default constructor
    security_attribute();
    // Copy constructor
    security_attribute(const security_attribute&);
    // Destructor
    ~security_attribute();
  private:
    ulong  _length;
    void*  _securityDescriptor;
    BOOL   _inheritHandle;
  };
}
