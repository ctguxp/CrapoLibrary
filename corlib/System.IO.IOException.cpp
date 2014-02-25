#include "pch.h"
#include "System.IO.IOException.h"

namespace System
  {
  namespace IO
    {
    IOException::IOException()
      :SystemException(L"I/O Error")
      {
      }
    IOException::IOException(cstring message)
      :SystemException(message)
      {
      }
    IOException::IOException(cstring message, Exception& innerException)
      :SystemException(message, innerException)
      {
      }
    IOException::IOException(cstring message, int hresult)
      :SystemException(message)
      {
      _hresult = hresult;
      }
    IOException::IOException(const IOException& e)
      :SystemException(e)
      {
      }
    IOException::~IOException()
      {
      }
    IOException& IOException::operator = (const IOException& e)
      {
      SystemException::operator=(e);
      return *this;
      }
    }
  }
