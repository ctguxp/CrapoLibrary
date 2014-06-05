#include "pch.h"
#include "System.ServiceProcess.ServiceManager.h"

namespace System
  {
  namespace ServiceProcess
    {
    ServiceManager::ServiceManager(String machineName, DWORD desiredAccess)
      :_handle(NULL)
      {
      _handle = ::OpenSCManager((cstring)machineName, SERVICES_ACTIVE_DATABASE, desiredAccess);
      }
    ServiceManager::~ServiceManager()
      {
      if(_handle != NULL)
        {
        ::CloseServiceHandle(_handle);
        }
      }
    }
  }