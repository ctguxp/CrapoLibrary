#include "pch.h"
#include "System.NotSupportedException.h"

namespace System
  {
  NotSupportedException::NotSupportedException(cstring message)
    :SystemException( (message != nullptr ? message : L"Operation is not supported.") )
    {
     _hresult = -2146233067;
    }
  NotSupportedException::NotSupportedException(cstring message, Exception& innerException)
    :SystemException( (message != nullptr ? message : L"Operation is not supported."), innerException)
    {
    }
  NotSupportedException::NotSupportedException(const NotSupportedException& e)
    :SystemException(e)
    {
    }
  NotSupportedException::~NotSupportedException()
    {
    }
  NotSupportedException& NotSupportedException::operator = (const NotSupportedException& e)
    {
    SystemException::operator=(e);
    return *this;
    }
  }
