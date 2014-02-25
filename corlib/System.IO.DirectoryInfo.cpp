#include "pch.h"
#include "System.IO.DirectoryInfo.h"
#include "System.IO.Path.h"
#include "System.Environment.h"
#include "System.Char.h"
#include "System.IO.Directory.h"

namespace System
  {
  namespace IO
    {
    DirectoryInfo::DirectoryInfo() 
      :FileSystemInfo()
      ,_current()
      ,_parent()
      {
      }
    DirectoryInfo::DirectoryInfo(String& path, bool simpleOriginalPath)
      :FileSystemInfo()
      ,_current()
      ,_parent()
      {
      CheckPath(path);

      // TODO : SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      _fullPath = Path::GetFullPath(path);
      if(simpleOriginalPath)
        _originalPath = Path::GetFileName(path);
      else
        _originalPath = path;

      Initialize();
      }
    DirectoryInfo::DirectoryInfo(const DirectoryInfo& di)
      :FileSystemInfo(di)
      {
      _current = di._current;
      _parent = di._parent;
      }
    DirectoryInfo& DirectoryInfo::operator= (const DirectoryInfo& di)
      {
      if(this == &di)
        return *this;

      FileSystemInfo::operator=(di);
      _current = di._current;
      _parent = di._parent;
      return *this;
      }
    DirectoryInfo::~DirectoryInfo()
      {
      }
    void DirectoryInfo::Create()
      {
      Directory::CreateDirectory(_fullPath);
      }
    DirectoryInfo DirectoryInfo::Parent()
      {
      if(_parent.Length() == 0)
        return DirectoryInfo();
      return DirectoryInfo(_parent);
      }
    bool DirectoryInfo::Exists()
      {
      Refresh(false);

      if(_stat.Attributes == FileAttributes::Invalid)
        return false;

      if( ((intptr)_stat.Attributes & (intptr)FileAttributes::Directory) == 0)
        return false;

      return true;
      }
    void DirectoryInfo::Initialize()
      {
      int len = _fullPath.Length() - 1;
      if((len > 1) && (_fullPath[len] == Path::DirectorySeparatorChar()))
        len--;
      int last = _fullPath.LastIndexOf(Path::DirectorySeparatorChar(), len);
      if((last == -1) || ((last == 0) && (len == 0)))
        {
        _current = _fullPath;
        }
      else 
        {
        _current = _fullPath.Substring(last + 1, len - last);
        if(last == 0 && !Environment::IsRunningOnWindows())
          _parent = Path::DirectorySeparatorStr();
        else
          _parent = _fullPath.Substring(0, last);
        // adjust for drives, i.e. a special case for windows
        if(Environment::IsRunningOnWindows()) 
          {
          if((_parent.Length() == 2) && (_parent [1] == ':') && Char::IsLetter(_parent[0]))
            _parent += Path::DirectorySeparatorChar();
          }
        }
      }
    }
  }
