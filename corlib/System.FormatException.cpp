#include "pch.h"
#include "System.FormatException.h"

namespace System
  {
  FormatException::FormatException(cstring message)
    :SystemException(message != nullptr ? message : L"Invalid format.")
    {
    _hresult = 0x80131537;
    }
  FormatException::~FormatException()
    {
    }
  }
