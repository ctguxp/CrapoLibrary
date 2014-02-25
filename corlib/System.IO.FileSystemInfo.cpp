#include "pch.h"
#include "System.IO.FileSystemInfo.h"
#include "System.Exception.h"
#include "System.IO.Path.h"
#include "System.Environment.h"
#include "System.IO.CrapoIO.h"

namespace System
  {
  namespace IO
    {
    FileSystemInfo::FileSystemInfo()
      :_fullPath()
		  ,_originalPath()
      ,_stat()
		  ,_valid(false)
      {
      memset(&_stat, 0, sizeof(CrapoIOStat));
      }
    FileSystemInfo::~FileSystemInfo()
      {
      }
    FileSystemInfo::FileSystemInfo(const FileSystemInfo& fsi)
      {
      _fullPath = fsi._fullPath;
      _originalPath = fsi._originalPath;
      }
    FileSystemInfo& FileSystemInfo::operator = (const FileSystemInfo& fsi)
      {
      if(this == &fsi)
        return *this;

      _fullPath = fsi._fullPath;
      _originalPath = fsi._originalPath;
      return *this;
      }
    void FileSystemInfo::CheckPath(String& path)
      {
      if(path.Length() == 0)
        throw ArgumentException(L"An empty file name is not valid.");
      if(path.IndexOfAny(Path::GetInvalidPathChars()) != -1)
        throw ArgumentException(L"Illegal characters in path.");
      if(Environment::IsRunningOnWindows()) 
        {
        int idx = path.IndexOf(L':');
        if (idx >= 0 && idx != 1)
          throw ArgumentException(L"path");
        }
      }
    void FileSystemInfo::Refresh(bool force)
		{
			if(_valid && !force)
				return;

			ulong error;
			
			CrapoIO::GetFileStat(_fullPath, _stat, error);
			/* Don't throw on error here, too much other
			 * stuff relies on it not doing so...
			 */
			
			_valid = true;
			
			InternalRefresh ();
		}
    void FileSystemInfo::InternalRefresh()
      {
      }
    }
  }
