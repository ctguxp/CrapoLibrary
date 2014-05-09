#include "pch.h"
#include "System.OutOfMemoryException.h"

namespace System
  {
  OutOfMemoryException::OutOfMemoryException(cstring message)
    :SystemException( (message != nullptr ? message : L"Out of memory.") )
    {
    _hresult = -2147024882;
    }
  OutOfMemoryException::OutOfMemoryException(cstring message, Exception& innerException)
    :SystemException( (message != nullptr ? message : L"Out of memory."), innerException)
    {
    _hresult = -2147024882;
    }
  OutOfMemoryException::OutOfMemoryException(const OutOfMemoryException& e)
    :SystemException(e)
    {
    }
  OutOfMemoryException::~OutOfMemoryException()
    {
    }
  OutOfMemoryException& OutOfMemoryException::operator = (const OutOfMemoryException& e)
    {
    SystemException::operator=(e);
    return *this;
    }
  }
