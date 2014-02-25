// Perch Lake Computer System

#include "pch.h"
#include "System.Environment.h"
#include "System.Array.h"
#include "System.Exception.h"
#include "System.IO.Directory.h"

namespace System
  {
  // Default constructor
  Environment::Environment()
    {
    }
  // Destructor
  Environment::~Environment()
    {
    }
  // Gets the NetBIOS name of this local computer.
  String Environment::MachineName()
    {
    // Allocate buffer
    ulong len = max_lib_buffer_size;
    CharArray buffer(len + 1);
    
    // Get computer name
    if(!::GetComputerName(buffer.ToPtr(), &len))
      throw WinException(L"Failed to get computer name");
    return String(buffer);
    }
  
  // ------------------------------------------------------------------------
  /// Gets or sets the fully qualified path of the current working directory. (Based on Mono)
  String Environment::CurrentDirectory()
    {
    return IO::Directory::GetCurrentDirectory();
    }
  void Environment::CurrentDirectory(String& path)
    {
    IO::Directory::SetCurrentDirectory(path);
    }
  // ------------------------------------------------------------------------

  // Gets the fully qualified path of the system directory.
  String Environment::SystemDirectory()
    {
    // Allocate buffer and get system directory
    CharArray buffer(max_lib_buffer_size + 1);
    ::GetSystemDirectory(buffer.ToPtr(), max_lib_buffer_size);
    return String(buffer);
    }
  // Gets the number of milliseconds elapsed since the system started.
  ulong Environment::TickCount()
    {
    return ::GetTickCount();
    }
  // Gets the user name of the person who is currently logged on to the Windows operating system
  String Environment::UserName()
    {
    // Allocate buffer
    ulong len = max_lib_buffer_size;
    CharArray buffer(len + 1);
    
    // Get user name
    ::GetUserName(buffer.ToPtr(), &len);
    return String(buffer);
    }

  bool Environment::IsRunningOnWindows()
    {
    return true;
    //return ((int) Platform < 4);
    }
  }
