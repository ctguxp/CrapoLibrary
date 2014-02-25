#include "pch.h"
#include "System.IO.DirectoryNotFoundException.h"

namespace System
  {
  namespace IO
    {
    DirectoryNotFoundException::DirectoryNotFoundException()
      :IOException(L"Directory not found")
      {
      }
    DirectoryNotFoundException::DirectoryNotFoundException(cstring message)
      :IOException(message)
      {
      }
    DirectoryNotFoundException::DirectoryNotFoundException(cstring message, Exception& innerException)
      :IOException(message, innerException)
      {
      }
    DirectoryNotFoundException::DirectoryNotFoundException(const IOException& e)
      :IOException(e)
      {
      }
    DirectoryNotFoundException::~DirectoryNotFoundException()
      {
      }
    DirectoryNotFoundException& DirectoryNotFoundException::operator = (const DirectoryNotFoundException& e)
      {
      IOException::operator=(e);
      return *this;
      }
    }
  }
