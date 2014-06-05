#include "pch.h"
#include "System.ServiceProcess.ServiceInstaller.h"
#include "System.ServiceProcess.ServiceController.h"
#include "System.ServiceProcess.ServiceManager.h"

namespace System
  {
  namespace ServiceProcess
    {
    ServiceInstaller::ServiceInstaller()
      :_startType(ServiceStartMode::Manual)
      ,_serviceName()
      ,_displayName()
      {
      }
    ServiceInstaller::~ServiceInstaller()
      {
      }
    String ServiceInstaller::DisplayName()
      {
      return _displayName;
      }
    void ServiceInstaller::DisplayName(String value)
      {
      _displayName = value;
      }
    String ServiceInstaller::ServiceName()
      {
      return _serviceName;
      }
    void ServiceInstaller::ServiceName(String value)
      {
      ServiceController::ValidateServiceName(value);
      _serviceName = value;
      }
    void ServiceInstaller::Install(String path)
      {
      ServiceManager manager(L".", SC_MANAGER_CREATE_SERVICE);
      SC_HANDLE handle = ::CreateService(manager, (cstring)_serviceName, (cstring)_displayName
        ,SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, (DWORD)_startType, SERVICE_ERROR_NORMAL, (cstring)path
        ,NULL, NULL, NULL, NULL, NULL);
      if(handle != NULL)
        {
        CloseServiceHandle(handle);
        return;
        }
      throw WinException(L"Failed to create service");
      //ERROR_ACCESS_DENIED
      }
    void ServiceInstaller::Uninstall()
      {
      ServiceManager manager(L".", SC_MANAGER_ALL_ACCESS);
      SC_HANDLE handle = ::OpenService(manager, (cstring)_serviceName, SERVICE_ALL_ACCESS);
      if(handle != NULL)
        {
        ::DeleteService(handle);
        CloseServiceHandle(handle);
        return;
        }
      throw WinException(L"Failed to delete service");
      }   
    }
  }