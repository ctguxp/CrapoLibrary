#include "pch.h"
#include "System.IO.FileNotFoundException.h"

namespace System
  {
  namespace IO
    {
    FileNotFoundException::FileNotFoundException()
      :IOException(L"Unable to find the specified file.")
      {
      _hresult = 0x80131621;
      }
    FileNotFoundException::FileNotFoundException(cstring message)
      :IOException(message)
      {
      _hresult = 0x80131621;
      }
    FileNotFoundException::FileNotFoundException(cstring message, Exception& innerException)
      :IOException(message, innerException)
      {
      _hresult = 0x80131621;
      }
    FileNotFoundException::FileNotFoundException(const FileNotFoundException& e)
      :IOException(e)
      {
      }
    FileNotFoundException::~FileNotFoundException()
      {
      }
    FileNotFoundException& FileNotFoundException::operator = (const FileNotFoundException& e)
      {
      IOException::operator=(e);
      return *this;
      }
    }
  }
