// Perch Lake Computer System

#include "pch.h"
#include "System.IO.File.h"
#include "System.Exception.h"
#include "System.IO.h"

namespace System
  {
  namespace IO
    {
    // ------------------------------------------------------------------------
    /// Default constructor
    File::File()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    //// Destructor
    File::~File()
      {
      }
    // ------------------------------------------------------------------------

    // Deletes the specified File.
    void File::Delete(System::String& path)
      {
      Path::Validate(path);
      if(Directory::Exists(path))
        throw UnauthorizedAccessException(L"Path is a directory");

      if(!::DeleteFile((cstring)path))
        throw System::WinException(L"Failed to remove file");
      }

    // ------------------------------------------------------------------------
    //// Determines whether the specified file exists (Based on Mono)
    bool File::Exists(System::String& path)
      {
      using namespace System;
      CharArray ipc = IO::Path::GetInvalidPathChars();
      if(String::IsNullOrWhiteSpace(path) || path.IndexOfAny(ipc) >= 0)
        return false;

      // on Moonlight this does not throw but returns false
      //if (!SecurityManager.CheckElevatedPermissions ())
      //return false;

      FileAttributes attrs = (FileAttributes)::GetFileAttributes((cstring)path);
      if(attrs == FileAttributes::Invalid)
        return false;

      if( ((ulong)attrs & (ulong)FileAttributes::Directory) != 0)
        return false;

      return true;
      }
    // ------------------------------------------------------------------------

    FileStream* File::Create(String& path)
      {
      return Create(path, 8192);
      }

    FileStream* File::Create(String& path, int bufferSize)
      {
      return new FileStream(path, FileMode::Create, FileAccess::ReadWrite,
                            FileShare::None, bufferSize);
      }

    FileStream* File::OpenRead(String path)
      {
      return new FileStream(path, FileMode::Open, FileAccess::Read, FileShare::Read);
      }
    }
  }
