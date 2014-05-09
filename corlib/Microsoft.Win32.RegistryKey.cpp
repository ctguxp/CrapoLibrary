#include "pch.h"
#include "Microsoft.Win32.RegistryKey.h"
#include "System.NotImplementedException.h"
#include "System.Int64.h"

namespace Microsoft
  {
  namespace Win32
    {
    RegistryKey::RegistryKey(RegistryHive hiveId, HKEY handle, bool remoteRoot)
      :_hive(hiveId)
      ,_handle( handle != NULL ? handle : (HKEY)(intptr)hiveId)
      ,_isRemoteRoot(remoteRoot)
      ,_isWritable(false)
      ,_qName(GetHiveName(hiveId))
      {
      }
    RegistryKey::RegistryKey(HKEY data, String keyName, bool writable)
      :_hive(RegistryHive::NullHive)
      ,_handle(data)
      ,_isRemoteRoot(false)
      ,_isWritable(writable)
      ,_qName(keyName)
      {
      }
    RegistryKey::~RegistryKey()
      {
      Flush();

      if(!_isRemoteRoot && IsRoot())
        return;

      ::RegCloseKey(_handle);
      }
    RegistryKey* RegistryKey::OpenSubKey(String name, bool writable)
      {
      int access = OpenRegKeyRead;
      if(writable)
        access |= OpenRegKeyWrite;

      HKEY subKeyHandle;
      LONG retval = ::RegOpenKeyEx(_handle, name, 0, access, &subKeyHandle);
      if(retval != ERROR_SUCCESS)
        throw System::WinException(L"Open Key Failed", retval);

      return new RegistryKey(subKeyHandle, CombineName((*this), name), writable);
      }
    String RegistryKey::CombineName(RegistryKey& rkey, String localName)
      {
      return String::Concat(rkey.Name(), L"\\", localName);
      }
    void RegistryKey::Flush()
      {
      if(_handle == NULL)
        return;
      ::RegFlushKey(_handle);
      }
    String RegistryKey::GetHiveName(RegistryHive hive)
      {
      switch (hive)
        {
        case RegistryHive::ClassesRoot:
          return "HKEY_CLASSES_ROOT";
        case RegistryHive::CurrentConfig:
          return "HKEY_CURRENT_CONFIG";
        case RegistryHive::CurrentUser:
          return "HKEY_CURRENT_USER";
        case RegistryHive::DynData:
          return "HKEY_DYN_DATA";
        case RegistryHive::LocalMachine:
          return "HKEY_LOCAL_MACHINE";
        case RegistryHive::PerformanceData:
          return "HKEY_PERFORMANCE_DATA";
        case RegistryHive::Users:
          return "HKEY_USERS";
        case RegistryHive::NullHive:
          break;
        }

      Int64 h((intptr)hive);
      throw NotImplementedException(String::Format(L"Registry hive '{0}' is not implemented.", &h));
      }
    }
  }
