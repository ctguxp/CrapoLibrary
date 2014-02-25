#pragma once
#include "System.String.h"
#include "System.IO.CrapoIOStat.h"

namespace System
  {
  namespace IO
    {
    class FileSystemInfo
      {
      protected:
        String      _fullPath;
		    String      _originalPath;
        CrapoIOStat _stat;
		    bool        _valid;
      public:
        FileSystemInfo();
        virtual ~FileSystemInfo();
        FileSystemInfo(const FileSystemInfo&);
        FileSystemInfo& operator = (const FileSystemInfo&);
        virtual bool Exists() = 0;
      protected:
        void CheckPath(String&);
        void Refresh(bool);
        virtual void InternalRefresh();
      };
    }
  }
