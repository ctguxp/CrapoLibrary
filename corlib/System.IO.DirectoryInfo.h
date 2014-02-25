#pragma once
#include "System.IO.FileSystemInfo.h"
#include "System.String.h"

namespace System
  {
  namespace IO
    {
    class DirectoryInfo : public FileSystemInfo
      {
      private:
        String _current;
		    String _parent;
      public:
        DirectoryInfo();
        DirectoryInfo(String& /*path*/, bool simpleOriginalPath = false);
        DirectoryInfo(const DirectoryInfo&);
        DirectoryInfo& operator= (const DirectoryInfo&);
        ~DirectoryInfo();
        DirectoryInfo Parent();
        void Create();
        virtual bool Exists() override;
      private:
        void Initialize();
      };
    }
  }
