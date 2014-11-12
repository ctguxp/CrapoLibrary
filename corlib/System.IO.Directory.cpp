// Perch Lake Computer System

#include "pch.h"
#include "System.IO.h"
#include "System.Exception.h"
#include "System.Environment.h"

namespace System
  {
  namespace IO
    {
    // ------------------------------------------------------------------------
    /// Default constructor
    Directory::Directory()
      {
      }
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    //// Destructor
    Directory::~Directory()
      {
      }
    // ------------------------------------------------------------------------

    DirectoryInfo Directory::CreateDirectory(String& path)
      {
      if(path.Length() == 0)
        throw ArgumentException(L"Path is empty");

      if(path.IndexOfAny(Path::GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Path contains invalid chars");

      if(path.Trim().Length() == 0)
        throw ArgumentException(L"Only blank characters in path");

      // after validations but before File.Exists to avoid an oracle
      // TODO :SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      if(File::Exists(path))
        {
        String msg(L"Cannot create {0} because a file with the same name already exists.");
        throw IOException(String::Format(msg, &path));
        }
        
      // LAMESPEC: with .net 1.0 version this throw NotSupportedException and msdn says so too
      // but v1.1 throws ArgumentException.
      if(Environment::IsRunningOnWindows() && path == String(L":"))
        throw ArgumentException(L"Only ':' In path");

      return CreateDirectoriesInternal(path);
      }

    void Directory::Delete(String& path)
      {
      Path::Validate(path);

      if(Environment::IsRunningOnWindows() && path == String(L":"))
        throw ArgumentException(L"Only ':' In path");
      // TODO : throw NotSupportedException ("Only ':' In path");

      // TODO : SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      ulong error;
      bool success;

      if(CrapoIO::ExistsSymlink(path, error)) 
        {
        /* RemoveDirectory maps to rmdir()
        * which fails on symlinks (ENOTDIR)
        */
        success = CrapoIO::DeleteFile(path, error);
        } 
      else 
        {
        success = CrapoIO::RemoveDirectory(path, error);
        }

      if(!success) 
        {
        /*
        * FIXME:
        * In io-layer/io.c rmdir returns error_file_not_found if directory does not exist.
        * So maybe this could be handled somewhere else?
        */
        if(error == ERROR_FILE_NOT_FOUND)
          {
          if(File::Exists(path))
            throw IOException(L"Directory does not exist, but a file of the same name exists.");
          else
            throw DirectoryNotFoundException(L"Directory does not exist.");
          } 
        else
          throw WinException(path, error);
        //throw MonoIO.GetException (path, error);
        }
      }

    void Directory::Delete(String& path, bool recursive)
      {
      Path::Validate(path);			
      // TODO : SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      if(recursive)
        RecursiveDelete(path);
      else
        Delete(path);
      }

    // ------------------------------------------------------------------------
    /// Gets the current working directory of the application.
    String Directory::GetCurrentDirectory()
      {
      return InsecureGetCurrentDirectory();
      }
    // ------------------------------------------------------------------------

    StringArray Directory::GetFiles(String& path)
      {
      String pattern(L"*");
      return GetFiles(path, pattern);
      }

    StringArray Directory::GetFiles(String& path, String& searchPattern)
      {
      return GetFileSystemEntries(path, searchPattern, FileAttributes::Directory, FileAttributes::None);
      }

    StringArray Directory::GetDirectories(String& path)
      {
      String pattern(L"*");
      return GetDirectories(path, pattern);
      }

    StringArray Directory::GetDirectories(String& path, String& searchPattern)
      {
      return GetFileSystemEntries(path, searchPattern, FileAttributes::Directory, FileAttributes::Directory);
      }

    void Directory::SetCurrentDirectory(String& path)
      {
      if(path.Trim().Length() == 0)
        ArgumentException(L"path string must not be an empty string or whitespace string");

      ulong error = 0;

      if (!Exists (path))
        {
        String msg("Directory \"{0}\" not found.");
        throw DirectoryNotFoundException(String::Format(msg, &path));
        }

      CrapoIO::SetCurrentDirectory(path, error);
      if (error != ERROR_SUCCESS)
        throw WinException(String::Empty(), error);
      }

    // ------------------------------------------------------------------------
    /// Determines whether the given path refers to an existing directory on disk. (Based on Mono)
    bool Directory::Exists(System::String& path)
      {
      // on Moonlight this does not throw but returns false
      //if (!SecurityManager.CheckElevatedPermissions ())
      //return false;

      ulong error;
      // No Exception thrown
      return CrapoIO::ExistsDirectory(path, error);
      }
    // ------------------------------------------------------------------------

    DirectoryInfo Directory::CreateDirectoriesInternal(String& path)
      {
      /*if (SecurityManager.SecurityEnabled) {
      new FileIOPermission (FileIOPermissionAccess.Read | FileIOPermissionAccess.Write, path).Demand ();
      }*/
      DirectoryInfo info(path, true);
      if(!info.Parent().Exists())
        info.Parent().Create();

      ulong error;
      if (!CrapoIO::CreateDirectory(path, error))
        {
        // LAMESPEC: 1.1 and 1.2alpha allow CreateDirectory on a file path.
        // So CreateDirectory ("/tmp/somefile") will succeed if 'somefile' is
        // not a directory. However, 1.0 will throw an exception.
        // We behave like 1.0 here (emulating 1.1-like behavior is just a matter
        // of comparing error to ERROR_FILE_EXISTS, but it's lame to do:
        //    DirectoryInfo di = Directory.CreateDirectory (something);
        // and having di.Exists return false afterwards.
        // I hope we don't break anyone's code, as they should be catching
        // the exception anyway.
        if (error != ERROR_ALREADY_EXISTS &&
          error != ERROR_FILE_EXISTS)
          throw WinException(path, error);
        // TODO : throw MonoIO.GetException (path, error);
        }

      return info;
      }

    void Directory::RecursiveDelete(String& path)
      {
      ulong error;
      int32 i = 0;

      StringArray df = GetDirectories(path);
      for(; i < (int32)df.Length(); ++i)
        {
        if(CrapoIO::ExistsSymlink(df[i], error))
          CrapoIO::DeleteFile(df[i], error);
        else
          RecursiveDelete(df[i]);
        }

      df = GetFiles(path);
      for(i = 0; i < (int32)df.Length(); ++i)
        File::Delete(df[i]);
      
      Directory::Delete(path);
      }

    StringArray Directory::GetFileSystemEntries(String& path, String& searchPattern, FileAttributes mask, FileAttributes attrs)
      {
      if(searchPattern.Length() == 0)
        return StringArray();
      bool stop;
      String path_with_pattern = ValidateDirectoryListing(path, searchPattern, stop);
      if(stop)
        {
        StringArray pwp(1);
        pwp[0] = path_with_pattern;
        return pwp;
        }

      ulong error;
      StringArray result = CrapoIO::GetFileSystemEntries(path, path_with_pattern, (int) attrs, (int) mask, error);
      if(error != 0)
        {
        String msg = Path::GetDirectoryName(Path::Combine(path, searchPattern));
        throw WinException(msg, error);
        //throw MonoIO.GetException (Path.GetDirectoryName (Path.Combine (path, searchPattern)), error);
        }

      return result;
      }

    // ------------------------------------------------------------------------
    /// Private InsecureGetCurrentDirectory function (Based on Mono)
    String Directory::InsecureGetCurrentDirectory()
      {
      ulong error = 0;
      GCString result(CrapoIO::GetCurrentDirectory(error));

      if (error != ERROR_SUCCESS)
        throw WinException(L"GetCurrentDirectory Failed", error);

      return *result.Get();
      }
    // ------------------------------------------------------------------------

    String Directory::ValidateDirectoryListing(String& path, String& searchPattern, bool& stop)
      {
      Path::Validate(path);

      String wild = Path::Combine(path, searchPattern);
      String wildpath = Path::GetDirectoryName(wild);
      if(wildpath.IndexOfAny(Path::GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Pattern contains invalid characters", L"pattern");

      ulong error;
      if (!CrapoIO::ExistsDirectory(wildpath, error))
        {
        if(error == ERROR_SUCCESS) 
          {
          ulong file_error;
          if(CrapoIO::ExistsFile(wildpath, file_error))
            throw IOException(L"The directory name is invalid.");
          }

        if(error != ERROR_PATH_NOT_FOUND)
          throw WinException(wildpath, error);
        //throw MonoIO.GetException(wildpath, error);

        CharArray searchPatterns((wchar_t*)SearchPatterns::WildcardChars, 2);
        if(wildpath.IndexOfAny(searchPatterns) == -1)
          {
          String msg(L"Directory '{0}' not found.");
          throw DirectoryNotFoundException(String::Format(msg, &wildpath));
          }

        if (path.IndexOfAny(searchPatterns) == -1)
          throw ArgumentException(L"Pattern is invalid", L"searchPattern");

        throw ArgumentException(L"Path is invalid", L"path");
        }

      stop = false;
      return wild;
      }

    }
  }
