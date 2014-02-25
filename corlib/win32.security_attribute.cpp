// Perch Lake Computer System

#include "pch.h"
#include "win32.security_attribute.h"

namespace win32
  {
  // Default constructor
  security_attribute::security_attribute()
    :_length(sizeof(SECURITY_ATTRIBUTES))
    ,_securityDescriptor(nullptr)
    ,_inheritHandle(TRUE)
    {
    }
    // Destructor
  security_attribute::~security_attribute()
    {
    }
  // Copy constructor
  security_attribute::security_attribute(const security_attribute& sa)
    :_length(sa._length)
    ,_securityDescriptor(sa._securityDescriptor)
    ,_inheritHandle(sa._inheritHandle)
    {
    }
  }
