#include "pch.h"
#include "System.ServiceProcess.ServiceController.h"
#include "System.ServiceProcess.ServiceManager.h"
#include "System.Exception.h"

namespace System
  {
  namespace ServiceProcess
    {

    ServiceController::ServiceHandle::ServiceHandle(String& name)
      :_name(name)
      ,_handle(NULL)
      {
      }

    ServiceController::ServiceHandle::~ServiceHandle()
      {
      if(_handle != NULL)
        ::CloseServiceHandle(_handle);
      }

    void ServiceController::ServiceHandle::Open(ServiceManager& manager, DWORD dwDesiredAccess)
      {
      _handle = ::OpenService(manager, (cstring)_name, dwDesiredAccess);
      if(_handle == NULL)
        throw WinException(L"Failed to open service");
      }

    ServiceController::ServiceController(cstring name, cstring machine)
      :_status()
      ,_name(name != nullptr ? name : L"")
      ,_serviceName()
      ,_machineName(machine != nullptr ? machine : L".")
      ,_displayName()
      {
      ZeroMemory(&_status, sizeof(SERVICE_STATUS_PROCESS));
      }
    ServiceController::ServiceController(const ServiceController& sc)
      :_status(sc._status)
      ,_name(sc._name)
      ,_serviceName(sc._serviceName)
      ,_machineName(sc._machineName)
      ,_displayName(sc._displayName)
      {
      }
    ServiceController::~ServiceController()
      {
      }
    ServiceController& ServiceController::operator=(const ServiceController& sc)
      {
      if(this == &sc)
        return *this;

      _status = sc._status;
      _name = sc._name;
      _serviceName = sc._serviceName;
      _machineName = sc._machineName;
      _displayName = sc._displayName;

      return *this;
      }
    void ServiceController::MachineName(String value)
      {
      _machineName = value;
      }
    String ServiceController::MachineName()
      {
      return _machineName;
      }
    String ServiceController::Name()
      {
      return _name;
      }
    String ServiceController::DisplayName()
      {
      String lookupName = _serviceName;

      try
        {
        ServiceManager manager(_machineName);
        if(lookupName.Length() == 0) 
          {
          // if the service name is not available, then
          // assume the specified name is in fact already a display
          // name
          try 
            {
            String serviceName = GetServiceName(manager, lookupName);
            _serviceName = serviceName;
            _name = String::Empty();
            return lookupName;
            } 
          catch(Exception) 
            {
            }
          }

        if(_displayName.Length() == 0)
          return GetServiceDisplayName(manager, lookupName, _machineName);
        } 
      catch(Exception)
        {
        }
      return _displayName;
      }
    void ServiceController::DisplayName(String value)
      {
      if (_displayName == value)
        return;

      _displayName = value;

      // if display name is modified, then we also need to force a
      // new lookup of the corresponding service name
      _serviceName = String::Empty();

      // you'd expect the DependentServices and ServiceDependedOn cache
      // to be cleared too, but the MS implementation doesn't do this
      //
      // categorized as by design:
      // https://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=201762

      // release any handles and clear cache
      //Close ();
      }
    String ServiceController::ServiceName()
      {
      String lookupName = _name;
      if(lookupName.Length() == 0)
        lookupName = _displayName;

      try
        {
        ServiceManager manager(_machineName);
        // assume the specified name is in fact a display name
        try
          {
          String serviceName = GetServiceName(manager, lookupName);
          _displayName = lookupName;
          _name = String::Empty();
          return serviceName;
          } 
        catch(Exception) 
          {
          }

        // instead of opening the service to verify whether it exists,
        // we'll try to get its displayname and hereby avoid looking
        // this up separately
        String displayName = GetServiceDisplayName(manager, lookupName, _machineName);
        _displayName = lookupName;
        _name = String::Empty();
        }
      catch(Exception)
        {
        }
      return lookupName;
      }
    void ServiceController::ServiceName(String value)
      {
      if(_serviceName == value)
        return;

      ValidateServiceName(value);

      _serviceName = value;

      // if service name is modified, then we also need to force a
      // new lookup of the corresponding display name
      _displayName = String::Empty();

      // you'd expect the DependentServices and ServiceDependedOn cache
      // to be cleared too, but the MS implementation doesn't do this
      //
      // categorized as by design:
      // https://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=201762

      // release any handles and clear cache
      //Close ();
      }
    ServiceControllerStatus ServiceController::Status() 
      {
      if ((int) _status.dwServiceType == 0)
        _status = GetServiceStatus(ServiceName(), _machineName);
      return (ServiceControllerStatus)_status.dwCurrentState;
      }
    SERVICE_STATUS_PROCESS ServiceController::GetServiceStatus(String& serviceName, String& machineName)
      {
      SERVICE_STATUS_PROCESS serviceStatus;
      ZeroMemory(&serviceStatus, sizeof(SERVICE_STATUS_PROCESS));

      ServiceManager manager(machineName, SC_MANAGER_CONNECT);

      ServiceHandle s(serviceName);
      s.Open(manager, SERVICE_QUERY_STATUS);

      DWORD bufferSize = 0;
      DWORD bytesNeeded = 0;
      ByteArray buffer;

      while (true) 
        {
        if (!::QueryServiceStatusEx(s, SC_STATUS_PROCESS_INFO, buffer.ToPtr(), bufferSize, &bytesNeeded)) {
          int err = ::GetLastError();
          if (err == ERROR_INSUFFICIENT_BUFFER) 
            {
            buffer.Length(bytesNeeded);
            bufferSize = bytesNeeded;
            }
          else 
            {
            throw WinException(L"Failed to query service status", err);
            }
          } 
        else 
          {
          memcpy(&serviceStatus, buffer.ToPtr(), sizeof(SERVICE_STATUS_PROCESS));
          break;
          }
        }

      return serviceStatus;
      }
    Array<ServiceController> ServiceController::GetServices()
      {
      ServiceController sc(L"dummy");
      String machineName(L".");
      return sc.GetServices(machineName, SERVICE_WIN32);
      }
    String ServiceController::GetServiceDisplayName(ServiceManager& scHandle, String& serviceName, String& machineName)
      {
      CharArray buffer;
      DWORD bufferSize = (DWORD)buffer.Length();

      while(true)
        {
        if(!::GetServiceDisplayName(scHandle, (cstring)serviceName, buffer.ToPtr(), &bufferSize))
          {
          int err = GetLastError();
          if(err == ERROR_INSUFFICIENT_BUFFER)
            {
            // allocate additional byte for terminating null char
            buffer.Length(bufferSize + 1);
            bufferSize = (uint32)buffer.Length();
            }
          else 
            {
            //throw new InvalidOperationException (string.Format(CultureInfo.CurrentCulture, "Service {0} was not" + " found on computer '{1}'.", serviceName, machineName), new Win32Exception ());
            throw SystemException(L"Service was not found on computer");
            }
          } 
        else 
          {
          return String(buffer);
          }
        }
      }
    String ServiceController::GetServiceName(ServiceManager& scHandle, String& displayName)
      {
      CharArray buffer;
      DWORD bufferSize = (DWORD)buffer.Length();

      while(true)
        {
        if(!::GetServiceKeyName(scHandle, (cstring)displayName, buffer.ToPtr(), &bufferSize)) 
          {
          int err = ::GetLastError();
          if(err == ERROR_INSUFFICIENT_BUFFER) 
            {
            // allocate additional byte for terminating null char
            buffer.Length(bufferSize + 1);
            bufferSize = (uint32)buffer.Length();
            } 
          else 
            {
            throw WinException(L"GetServiceKeyName Failed");
            }
          } 
        else 
          {
          return String(buffer.ToConstPtr());
          }
        }
      }
    void ServiceController::ValidateServiceName(String& serviceName)
      {
      if(serviceName.Length() == 0 || serviceName.Length() > 80)
        //throw new ArgumentException (string.Format (CultureInfo.CurrentCulture, "Service name {0} contains invalid characters, is empty" + " or is too long (max length = 80).", serviceName));
        throw ArgumentException(L"Service name is empty or contains invalid characters.");
      }
    void ServiceController::ValidateMachineName(String machineName)
		{
			if(machineName.Length() == 0)
				//throw new ArgumentException (string.Format (CultureInfo.CurrentCulture, "MachineName value {0} is invalid.", machineName));
        throw ArgumentException(L"MachineName is invalid");
		}
    Array<ServiceController> ServiceController::GetServices(String& machineName, DWORD serviceType, cstring group)
      {
      ByteArray buffer;



      ServiceManager manager(machineName, SC_MANAGER_ENUMERATE_SERVICE);

      DWORD bufferSize = 0;
      DWORD bytesNeeded = 0;
      DWORD servicesReturned = 0;
      DWORD resumeHandle = 0;

      Array<ServiceController> services;

      while (true) 
        {
        if (!::EnumServicesStatusEx(manager, SC_ENUM_PROCESS_INFO, serviceType, SERVICE_STATE_ALL, buffer.ToPtr(), bufferSize, &bytesNeeded, &servicesReturned, &resumeHandle, group))
          {
          int err = GetLastError();
          if(err == ERROR_MORE_DATA) 
            {
            buffer.Length(bytesNeeded);
            bufferSize = bytesNeeded;
            } 
          else 
            {
            throw WinException(L"Failed to enumerate services", err);
            // TODO throw new Win32Exception(err);
            }
          } 
        else 
          {
          byte* ptr = buffer.ToPtr();

          services.Length(servicesReturned);
          for(DWORD i = 0; i < servicesReturned; i++) 
            {
            ENUM_SERVICE_STATUS_PROCESS serviceStatus;
            memcpy(&serviceStatus, ptr, sizeof(ENUM_SERVICE_STATUS_PROCESS));
            services[i] = ServiceController(serviceStatus.lpServiceName, machineName);
            ptr += sizeof(ENUM_SERVICE_STATUS_PROCESS);
            }

          // we're done, so exit the loop
          break;
          }
        }

      return services;
      }
    }
  }
