// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.IO.FileAttributes.h"
#include "System.IO.Enums.h"
#include "System.IO.CrapoFileType.h"
#include "System.IO.SeekOrigin.h"
#include "System.IO.CrapoIOStat.h"

namespace System
  {
  namespace IO
    {
    class CrapoIO
      {
      public:
        CrapoIO();
        ~CrapoIO();
        
        static bool CreateDirectory(String&, ulong&);
        static bool RemoveDirectory(String&, ulong&);
        static String* GetCurrentDirectory(ulong&);
        static bool SetCurrentDirectory(String&, ulong&);

        static bool MoveFile(String&, String&, ulong&);
        static bool CopyFile(String&, String&, bool, ulong&);
        static bool DeleteFile(String&, ulong&);
        static FileAttributes GetFileAttributes(String&, ulong&);
        static bool GetFileStat(String& path, CrapoIOStat& stat, ulong& error);
        static CrapoFileType GetFileType(HANDLE handle, ulong& error);
        static StringArray GetFileSystemEntries(String& path, String& path_with_pattern, int attrs, int mask, ulong& error);

        static bool Exists(String&, ulong&);
        static bool ExistsDirectory(String&, ulong&);
        static bool ExistsFile(String& path, ulong& error);
        static bool ExistsSymlink(String& path, ulong& error);

        static HANDLE Open(String& filename, FileMode mode, FileAccess access, FileShare share, FileOptions options, ulong& error);
        static bool Close(HANDLE handle, ulong& error);
        static int Read(HANDLE handle, ByteArray& dest, int dest_offset, int count, ulong& error);
        static intptr GetLength(HANDLE handle, ulong& error);
        static intptr Seek(HANDLE handle, int64 offset, SeekOrigin origin, ulong& error);
        static int Write(HANDLE handle, ByteArray& src, int src_offset, int count, ulong& error);
      private:
        static BOOL get_file_attributes_ex(String& path, WIN32_FILE_ATTRIBUTE_DATA& data);
        static int64 convert_filetime(const FILETIME& filetime);
        static int GetFileAttributes(String& path);    
      };
    }
  }
