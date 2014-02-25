// Perch Lake Computer System
// Written by Mark Mohr (markm11@charter.net)

#pragma once
#include "System.String.h"

namespace System
  {
  namespace IO
    {
    class File;
    class Directory;
    class DirectoryInfo;
    class CRAPOCOREDLL_API Path
      {
      public:
        Path();
        ~Path();
        
        // Methods
        static String ChangeExtension(String&, String*);
        static String Combine(String path1, String path2);
        static String GetDirectoryName(String);
        static String GetFileName(String);
        static String GetFileNameWithoutExtension(String);
        static String GetFullPath(String& path);
        static CharArray GetInvalidPathChars();
        static String GetPathRoot(String&);
        static String GetTempPath();
        static bool IsPathRooted(String&);

        // Fields
        static wchar_t AltDirectorySeparatorChar() { return L'/'; }
        static wchar_t DirectorySeparatorChar() { return L'\\'; }
        static wchar_t VolumeSeparatorChar() { return L':'; }
        static wchar_t InvalidPathChars[36];

     private:
        static String CanonicalizePath(String path);
        static String CleanPath(String&);
        static String DirectorySeparatorStr();
        static String GetServerAndShare(String path);
        static String InsecureGetFullPath(String& path);
        static bool IsDsc(wchar_t);
        static CharArray PathSeparatorChars();
        static bool SameRoot(String& root, String& path);
        static void Validate(String&);
        static void Validate(String&, String&);  
        static String get_temp_path();      
        static int findExtension(String);
        static bool dirEqualsVolume();
        static String WindowsDriveAdjustment(String&);
        static String GetFullPathName(String&);

      friend File;
      friend Directory;
      friend DirectoryInfo;
      };
    }
  }
