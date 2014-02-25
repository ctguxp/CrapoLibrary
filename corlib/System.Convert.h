// Perch Lake Computer System

#pragma once
#include "System.Array.h"
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Convert
    {
    public:
      // Default constructor
      Convert();
      // Destructor
      ~Convert();
      // From base64 string;
      static System::ByteArray from_base64_string(System::String&);
      // To base64 string
      //static System::String to_base64_string(System::ByteArray&);
    };
  }
