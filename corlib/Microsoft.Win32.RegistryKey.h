#pragma once
#include "Microsoft.Win32.RegistryHive.h"
#include "System.String.h"

using namespace System;

namespace Microsoft
  {
  namespace Win32
    {
    class RegistryKey
      {
      private:
        enum
          {
          OpenRegKeyRead = KEY_READ,
          OpenRegKeyWrite = KEY_WRITE
          };
      public:
        RegistryKey(RegistryHive hiveId, HKEY handle = NULL, bool remoteRoot = false);
        RegistryKey(HKEY data, String keyName, bool writable);
        ~RegistryKey();
        void Flush();
        bool IsRoot() { return _hive != RegistryHive::NullHive; }
        String Name() { return _qName; }
        RegistryKey* OpenSubKey(String name, bool writable = false);
      private:
        static String CombineName(RegistryKey& rkey, String localName);
        static String GetHiveName(RegistryHive hive);
      private:
        RegistryHive _hive;
        HKEY         _handle;
        bool         _isRemoteRoot;
        bool         _isWritable;
        String       _qName;               
      };
    typedef AutoPtr<RegistryKey> GCRegistryKey;
    }
  }
