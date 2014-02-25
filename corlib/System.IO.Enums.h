#pragma once

namespace System
  {
  namespace IO
    {
    enum class FileAccess
      {
      Read =  1,
      Write = 2,
      ReadWrite = 3
      };

    enum class FileMode
      {
      CreateNew = 1,   // Specifies that the operating system should create a new file
      Create,           // Specifies that the operating system should create a new file. If the file already exists, it will be overwritten
      Open,             // Specifies that the operating system should open an existing file
      OpenOrCreate,
      Truncate,
      Append
      };

    enum class FileShare
      {
      None = 0,
      Read = 1,
      Write = 2,
      ReadWrite = 3,
      Delete = 4,
      Inheritable = 16
      };

    enum FileOptions
      {
      None = 0,
      Temporary = 1,
      WriteThrough = 0xF3333334,
      Asynchronous = 0x40000000,
      RandomAccess = 0x10000000,
      DeleteOnClose = 67108864,
      SequentialScan = 134217728,
      Encrypted = 16384
      };

    }
  }