// Perch Lake Computer System
// Written by Mark Mohr (markm11@charter.net)

#include "pch.h"
#include "System.IO.Path.h"
#include "System.Exception.h"
#include "System.Environment.h"
#include "System.Char.h"
#include "System.IO.Directory.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Text.StringBuilder.h"

namespace System
  {
  namespace IO
    {

    wchar_t Path::InvalidPathChars[36] = {
      '\x22', '\x3C', '\x3E', '\x7C', '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06',
      '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F', '\x10', '\x11',
      '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', 
      '\x1D', '\x1E', '\x1F' };

    // ------------------------------------------------------------------------
    /// Default constructor
    Path::Path()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Destructor
    Path::~Path()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Changes the extension of a path string. (Based on Mono)
    String Path::ChangeExtension(String& path, String* extension)
      {
        {
        System::CharArray iv = GetInvalidPathChars();
        if (path.IndexOfAny(iv) != -1)
          throw System::ArgumentException(L"Illegal characters in path.");
        }

        int iExt = findExtension(path);

        System::String dot(L".");
        if (extension == nullptr)
          return iExt < 0 ? path : path.Substring (0, iExt);
        else if(extension->Length() == 0)
          return iExt < 0 ? path + dot : path.Substring (0, iExt + 1);

        if(path.Length() != 0)
          {
          if(extension->Length() > 0 && (*extension)[0] != '.')
            *extension = dot + *extension;
          } 
        else
          {
          System::String e;
          *extension = e;
          }

        if (iExt < 0)
          {
          return path + *extension;
          } 
        else if (iExt > 0)
          {
          System::String temp = path.Substring (0, iExt);
          return temp + *extension;
          }
        return *extension;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Returns the directory information for the specified path string.
    String Path::GetDirectoryName(String path)
      {
      if(GetPathRoot(path) == path)
        return String::Empty();

      if(path.Trim().Length() == 0)
        throw ArgumentException(L"Argument string consists of whitespace characters only.");

      if(path.IndexOfAny(GetInvalidPathChars()) > -1)
        throw ArgumentException(L"Path contains invalid characters");

      int nLast = path.LastIndexOfAny(PathSeparatorChars());
      if(nLast == 0)
        nLast++;

      if (nLast > 0)
        {
        String ret = path.Substring(0, nLast);
        int l = ret.Length();

        if(l >= 2 && DirectorySeparatorChar() == '\\' && ret [l - 1] == VolumeSeparatorChar())
          return ret + String(DirectorySeparatorChar());
        else if(l == 1 && DirectorySeparatorChar() == '\\' && path.Length() >= 2 && path[nLast] == VolumeSeparatorChar())
          return ret + String(VolumeSeparatorChar());
        else
          {
          //
          // Important: do not use CanonicalizePath here, use
          // the custom CleanPath here, as this should not
          // return absolute paths
          //
          return CleanPath(ret);
          }
        }
      return String::Empty();
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Returns the file name and extension of the specified path string. (Based on Mono)
    String Path::GetFileName(String path)
      {
      if(path.Length() == 0)
        return path;

        { // Check for invalid path chars
        System::CharArray iv = GetInvalidPathChars();
        if(path.IndexOfAny(iv) != -1)
          throw System::ArgumentException(L"Illegal characters in path.");
        }

        { // Find last path separator and return path
        System::CharArray psc = PathSeparatorChars();
        int nLast = path.LastIndexOfAny(psc);
        if (nLast >= 0)
          return path.Substring(nLast + 1);
        }

        return path;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Returns the file name of the specified path string without the extension. (Based on Mono)
    String Path::GetFileNameWithoutExtension(String path)
      {
      path = GetFileName(path);
      return ChangeExtension(path, nullptr);
      }
    // ------------------------------------------------------------------------

    /// Returns the absolute path for the specified path string.
    String Path::GetFullPath(String& path)
      {
      String fullpath = InsecureGetFullPath(path);

      // TODO: SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      return fullpath;
      }

    // ------------------------------------------------------------------------
    /// Gets an array containing the characters that are not allowed in path names. (Based on Mono)
    CharArray Path::GetInvalidPathChars()
      {
      // return a new array as we do not want anyone to be able to change the values
      if (Environment::IsRunningOnWindows()) 
        {
        CharArray winRet((wchar_t*)InvalidPathChars, 36); 
        return winRet;        
        }

      CharArray retval(1);
      retval[0] = L'\x00';
      return retval;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Gets the root directory information of the specified path.
    String Path::GetPathRoot(String& path)
      {
      if(path.Trim().Length() == 0)
        throw ArgumentException(L"The specified path is not of a legal form.");

      if(!IsPathRooted(path))
        return String::Empty();

      if(DirectorySeparatorChar() == L'/')
        {
        // UNIX
        return IsDsc(path [0]) ? DirectorySeparatorStr() : String::Empty();
        } 
      else 
        {
        // Windows
        int len = 2;

        if (path.Length() == 1 && IsDsc (path [0]))
          return DirectorySeparatorStr();
        else if (path.Length() < 2)
          return String::Empty();

        if (IsDsc(path [0]) && IsDsc(path[1])) 
          {
          // UNC: \\server or \\server\share
          // Get server
          while(len < path.Length() && !IsDsc(path [len])) len++;

          // Get share
          if(len < path.Length()) 
            {
            len++;
            while (len < path.Length() && !IsDsc(path [len])) len++;
            }

          return DirectorySeparatorStr() + DirectorySeparatorStr() + path.Substring(2, len - 2).Replace(AltDirectorySeparatorChar(), DirectorySeparatorChar());
          } 
        else if(IsDsc (path [0]))
          {
          // path starts with '\' or '/'
          return DirectorySeparatorStr();
          } 
        else if(path[1] == VolumeSeparatorChar())
          {
          // C:\folder
          if(path.Length() >= 3 && (IsDsc(path [2]))) len++;
          } 
        else
          return Directory::GetCurrentDirectory().Substring(0, 2);// + path.Substring (0, len);
        return path.Substring(0, len);
        }
      }
    // ------------------------------------------------------------------------

    String Path::GetTempPath()
      {
      //SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      String p = get_temp_path ();
      if(p.Length() > 0 && p [p.Length() - 1] != DirectorySeparatorChar())
        return p + String(DirectorySeparatorChar());

      return p;
      }

    // ------------------------------------------------------------------------
    /// Gets a value indicating whether the specified path string contains a root.
    bool Path::IsPathRooted(String& path)
      {
      if(path.Length() == 0)
        return false;

      if(path.IndexOfAny(GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Illegal characters in path.");

      wchar_t c = path[0];
      return(c == DirectorySeparatorChar() || c == AltDirectorySeparatorChar() ||
        (!dirEqualsVolume() && path.Length() > 1 && path[1] == VolumeSeparatorChar()));
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private CleanPath function (Base on Mono)
    String Path::CleanPath(String& s)
      {
      int l = s.Length();
      int sub = 0;
      int start = 0;

      // Host prefix?
      wchar_t s0 = s[0];
      if(l > 2 && s0 == L'\\' && s [1] == L'\\')
        {
        start = 2;
        }

      // We are only left with root
      if(l == 1 && (s0 == DirectorySeparatorChar() || s0 == AltDirectorySeparatorChar()))
        return s;

      // Cleanup
      for(int i = start; i < l; i++)
        {
        wchar_t c = s[i];

        if(c != DirectorySeparatorChar() && c != AltDirectorySeparatorChar())
          continue;
        if (i+1 == l)
          sub++;
        else 
          {
          c = s[i + 1];
          if(c == DirectorySeparatorChar() || c == AltDirectorySeparatorChar())
            sub++;
          }
        }

      if(sub == 0)
        return s;

      CharArray copy(l-sub);
      if (start != 0)
        {
        copy[0] = '\\';
        copy[1] = '\\';
        }
      for(int i = start, j = start; i < l && j < (int)copy.Length(); i++)
        {
        wchar_t c = s [i];

        if(c != DirectorySeparatorChar() && c != AltDirectorySeparatorChar())
          {
          copy[j++] = c;
          continue;
          }

        // For non-trailing cases.
        if(j+1 != (int)copy.Length())
          {
          copy[j++] = DirectorySeparatorChar();
          for(;i < l-1; i++)
            {
            c = s[i+1];
            if(c != DirectorySeparatorChar() && c != AltDirectorySeparatorChar())
              break;
            }
          }
        }
      return String(copy);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private dirEqualsVolume function (Base on Mono)
    bool Path::dirEqualsVolume()
      {
      return (DirectorySeparatorChar() == VolumeSeparatorChar());
      }
    // ------------------------------------------------------------------------

    String Path::get_temp_path()
      {
      CharArray temp(max_lib_buffer_size + 1);
      ulong retval = ::GetTempPath(max_lib_buffer_size, temp.ToPtr());
      if(retval == 0)
        throw WinException(L"Failed to get temp path");
      return String(temp.ToConstPtr());
      }

    // ------------------------------------------------------------------------
    /// Private findExtension function (Base on Mono)
    int Path::findExtension(String path)
      {
      // method should return the index of the path extension
      // start or -1 if no valid extension
      if (path.Length() != 0)
        {
        int iLastDot = path.LastIndexOf('.');
        System::CharArray psc = PathSeparatorChars();
        int iLastSep = path.LastIndexOfAny(psc);

        if (iLastDot > iLastSep)
          return iLastDot;
        }
      return -1;
      }
    // ------------------------------------------------------------------------

    bool Path::SameRoot(String& root, String& path)
      {
      // compare root - if enough details are available
      if ((root.Length() < 2) || (path.Length() < 2))
        return false;

      // UNC handling
      if (IsDsc (root[0]) && IsDsc (root[1]))
        {
        if (!(IsDsc (path[0]) && IsDsc (path[1])))
          return false;

        String rootShare = GetServerAndShare(root);
        String pathShare = GetServerAndShare(path);

        // TODO : return String::Compare(rootShare, pathShare, true, Globalization::CultureInfo::InvariantCulture) == 0;
        return 0;
        }

      // same volume/drive
      if (!Char(root[0]).Equals(Char(path[0])))
        return false;
      // presence of the separator
      if (path[1] != Path::VolumeSeparatorChar())
        return false;
      if ((root.Length() > 2) && (path.Length() > 2))
        {
        // but don't directory compare the directory separator
        return (IsDsc(root[2]) && IsDsc(path[2]));
        }
      return true;
      }

    String Path::CanonicalizePath(String path)
      {
      // STEP 1: Check for empty string
      if (Environment::IsRunningOnWindows())
        path = path.Trim();

      if(path.Length() == 0)
        return path;

      // STEP 2: Check to see if this is only a root
      String root = Path::GetPathRoot(path);
      // it will return '\' for path '\', while it should return 'c:\' or so.
      // Note: commenting this out makes the need for the (target == 1...) check in step 5
      //if (root == path) return path;

      // STEP 3: split the directories, this gets rid of consecutive "/"'s
      CharArray seps(2);
      seps[0] = Path::DirectorySeparatorChar();
      seps[1] = Path::AltDirectorySeparatorChar();
      StringArray dirs = path.Split(seps);
      // STEP 4: Get rid of directories containing . and ..
      int target = 0;

      bool isUnc = Environment::IsRunningOnWindows() &&
        root.Length() > 2 && IsDsc(root[0]) && IsDsc(root[1]);

      // Set an overwrite limit for UNC paths since '\' + server + share
      // must not be eliminated by the '..' elimination algorithm.
      int limit = isUnc ? 3 : 0;

      for(uint32 i = 0; i < dirs.Length(); i++) 
        {
        // WIN32 path components must be trimmed
        if(Environment::IsRunningOnWindows())
          dirs[i] = dirs[i].TrimEnd();


        if(dirs[i] == String(L".") || (i != 0 && dirs[i].Length() == 0))
          continue;
        else if (dirs[i] == "..") 
          {
          // don't overwrite path segments below the limit
          if(target > limit)
            target--;
          } 
        else
          dirs[target++] = dirs[i];
        }

      // STEP 5: Combine everything.
      if(target == 0 || (target == 1 && dirs[0] == ""))
        return root;
      else 
        {
        String ret = String::Join(DirectorySeparatorStr(), dirs, 0, target);
        if(Environment::IsRunningOnWindows()) 
          {
          // append leading '\' of the UNC path that was lost in STEP 3.
          if (isUnc)
            ret = Path::DirectorySeparatorStr() + ret;

          if(!SameRoot(root, ret))
            ret = root + ret;

          if (isUnc)
            {
            return ret;
            } 
          else if(!IsDsc (path[0]) && SameRoot(root, path)) 
            {
            if(ret.Length() <= 2 && !ret.EndsWith(DirectorySeparatorStr())) // '\' after "c:"
              ret += Path::DirectorySeparatorChar();
            return ret;
            } 
          else 
            {
            String current = Directory::GetCurrentDirectory();
            if(current.Length() > 1 && current[1] == Path::VolumeSeparatorChar()) 
              {
              // DOS local file path
              if(ret.Length() == 0 || IsDsc(ret[0]))
                ret += String(L'\\');
              return current.Substring(0, 2) + ret;
              } 
            else if(IsDsc(current[current.Length() - 1]) && IsDsc (ret[0]))
              return current + ret.Substring(1);
            else
              return current + ret;
            }
          } 
        else 
          {
          if(root != "" && ret.Length() > 0 && ret [0] != '/')
            ret = root + ret;
          }
        return ret;
        }
      }

    // ------------------------------------------------------------------------
    /// Private findExtension function (Base on Mono)
    String Path::DirectorySeparatorStr()
      {
      Char ret(DirectorySeparatorChar());
      return ret.ToString();
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private GetServerAndShare function (Base on Mono)
    String Path::GetServerAndShare(String path)
      {
      int len = 2;
      while(len < path.Length() && !IsDsc(path[len])) len++;

      if(len < path.Length())
        {
        len++;
        while(len < path.Length() && !IsDsc(path [len])) len++;
        }

      return path.Substring(2, len - 2).Replace(AltDirectorySeparatorChar(), DirectorySeparatorChar());
      }
    // ------------------------------------------------------------------------

    String Path::InsecureGetFullPath(String& path)
      {
      if(path.Trim().Length() == 0) 
        throw ArgumentException(L"The specified path is not of a legal form (empty).");

      // adjust for drives, i.e. a special case for windows
      if(Environment::IsRunningOnWindows())
        path = WindowsDriveAdjustment(path);

      // if the supplied path ends with a separator...
      wchar_t end = path[path.Length() - 1];

      auto canonicalize = true;
      if(path.Length() >= 2 && IsDsc(path[0]) && IsDsc(path[1]))
        {
        if(path.Length() == 2 || path.IndexOf(path[0], 2) < 0)
          throw ArgumentException(L"UNC paths should be of the form \\\\server\\share.");

        if(path[0] != DirectorySeparatorChar())
          path = path.Replace(AltDirectorySeparatorChar(), DirectorySeparatorChar());

        } 
      else 
        {
        if(!IsPathRooted(path)) 
          {

          // avoid calling expensive CanonicalizePath when possible
          if(!Environment::IsRunningOnWindows())
            {
            auto start = 0;
            while((start = path.IndexOf(L'.', start)) != -1)
              {
              if (++start == path.Length() || path[start] == DirectorySeparatorChar() || path[start] == AltDirectorySeparatorChar())
                break;
              }
            canonicalize = start > 0;
            }

          path = Directory::InsecureGetCurrentDirectory() + DirectorySeparatorStr() + path;
          } 
        else if(DirectorySeparatorChar() == L'\\' &&
          path.Length() >= 2 &&
          IsDsc(path[0]) &&
          !IsDsc(path[1])) 
          { // like `\abc\def'
          String current = Directory::InsecureGetCurrentDirectory();
          if(current [1] == VolumeSeparatorChar())
            path = current.Substring (0, 2) + path;
          else
            path = current.Substring(0, current.IndexOf(L'\\', current.IndexOfOrdinalUnchecked(L"\\\\") + 1));
          }
        }

      if(canonicalize)
        path = CanonicalizePath(path);

      // if the original ended with a [Alt]DirectorySeparatorChar then ensure the full path also ends with one
      if(IsDsc(end) && (path[path.Length() - 1] != DirectorySeparatorChar()))
        path += DirectorySeparatorChar();

      return path;
      }

    String Path::WindowsDriveAdjustment(String& path)
      {
      // two special cases to consider when a drive is specified
      if(path.Length() < 2)
        return path;
      if((path[1] != L':') || !Char::IsLetter(path [0]))
        return path;

      String current = Directory::InsecureGetCurrentDirectory();
      // first, only the drive is specified
      if (path.Length() == 2) 
        {
        // then if the current directory is on the same drive
        if(current [0] == path[0])
          path = current; // we return it
        else
          path = GetFullPathName(path); // we have to use the GetFullPathName Windows API
        } 
      else if((path [2] != Path::DirectorySeparatorChar()) && (path [2] != Path::AltDirectorySeparatorChar()))
        {
        // second, the drive + a directory is specified *without* a separator between them (e.g. C:dir).
        // If the current directory is on the specified drive...
        if(current [0] == path[0]) 
          {
          // then specified directory is appended to the current drive directory
          path = Path::Combine(current, path.Substring (2, path.Length() - 2));
          } 
        else 
          {
          // we have to use the GetFullPathName Windows API
          path = GetFullPathName(path);
          }
        }
      return path;
      }

    String Path::GetFullPathName(String& path)
      {
      using namespace Text;
      CharArray buffer(max_lib_buffer_size);
      ::GetFullPathName(path, max_lib_buffer_size - 1, buffer.ToPtr(), nullptr);
      return String(buffer.ToConstPtr());
      }

    String Path::Combine(String path1, String path2)
      {
      if(path1.Length() == 0)
        return path2;

      if(path2.Length() == 0)
        return path1;

      if(path1.IndexOfAny(GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Illegal characters in path.");

      if(path2.IndexOfAny(GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Illegal characters in path.");

      //TODO???: UNC names
      if(IsPathRooted (path2))
        return path2;

      wchar_t p1end = path1[path1.Length() - 1];
      if(p1end != DirectorySeparatorChar() && p1end != AltDirectorySeparatorChar() && p1end != VolumeSeparatorChar())
        return path1 + DirectorySeparatorStr() + path2;

      return path1 + path2;
      }

    // ------------------------------------------------------------------------
    /// Private IsDsc function (Base on Mono)
    bool Path::IsDsc(wchar_t c)
      {
      return c == DirectorySeparatorChar() || c == AltDirectorySeparatorChar();
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private PathSepartorChars functin (Base on Mono)
    CharArray Path::PathSeparatorChars()
      {
      System::CharArray retval(3);
      retval[0] = DirectorySeparatorChar();
      retval[1] = AltDirectorySeparatorChar();
      retval[2] = VolumeSeparatorChar();
      return retval;
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private Validate path function (Base on Mono)
    void Path::Validate(String& path)
      {
      String param(L"path");
      Validate(path, param);
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    /// Private Validate path function (Base on Mono)
    void Path::Validate(String& path, String& parameterName)
      {
      /*if(path.IsNull())
      throw ArgumentNullException(parameterName.get_buffer());*/
      if(String::IsNullOrWhiteSpace(path))
        throw ArgumentException(L"Path is empty");
      CharArray ipc = GetInvalidPathChars();
      if(path.IndexOfAny(ipc) != -1)
        throw ArgumentException(L"Path contains invalid chars");
      if(Environment::IsRunningOnWindows())
        {
        int idx = path.IndexOf(':');
        if(idx >= 0 && idx != 1)
          throw ArgumentException(parameterName);
        }
      }
    // ------------------------------------------------------------------------

    }
  }
