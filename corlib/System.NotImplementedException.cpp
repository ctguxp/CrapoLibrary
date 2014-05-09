#include "pch.h"
#include "System.NotImplementedException.h"

namespace System
  {
  NotImplementedException::NotImplementedException(cstring message)
    :SystemException( (message != nullptr ? message : L"The requested feature is not implemented") )
    {
    _hresult = -2147467263;
    }

  NotImplementedException::NotImplementedException(cstring message, Exception& innerException)
    :SystemException( (message != nullptr ? message : L"The requested feature is not implemented"), innerException)
    {
    _hresult = -2147467263;
    }

  NotImplementedException::NotImplementedException(const NotImplementedException& e)
    :SystemException(e)
    {
    }

  NotImplementedException::~NotImplementedException()
    {
    }

  NotImplementedException& NotImplementedException::operator = (const NotImplementedException& e)
    {
    SystemException::operator=(e);
    return *this;
    }
  }
