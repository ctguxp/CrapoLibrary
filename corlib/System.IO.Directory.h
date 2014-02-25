// Perch Lake Computer System

#pragma once
#include "System.String.h"
#include "System.IO.DirectoryInfo.h"

namespace System
  {
  namespace IO
    {
    class Path;
    class Directory
      {
      public:
        Directory();
        ~Directory();

        // Methods
        static DirectoryInfo CreateDirectory(String& path);
        static void Delete(String& path);
        static void Delete(String& path, bool recursive);
        static String GetCurrentDirectory();
        static StringArray GetFiles(String& path);
        static StringArray GetFiles(String& path, String& searchPattern);
        static StringArray GetDirectories(String& path);
        static StringArray GetDirectories(String& path, String& searchPattern);
        static bool Exists(System::String&);
        static void SetCurrentDirectory(String& path);

      private:
        static StringArray GetFileSystemEntries(String& path, String& searchPattern, FileAttributes mask, FileAttributes attrs);
        static void RecursiveDelete(String& path);
        static DirectoryInfo CreateDirectoriesInternal(String& path);
        static String Directory::InsecureGetCurrentDirectory();
        static String ValidateDirectoryListing(String& path, String& searchPattern, bool& stop);

      friend Path;
      };
    }
  }    
