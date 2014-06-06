#include "pch.h"
#include "System.ServiceProcess.ServiceController.h"

namespace System
  {
  namespace ServiceProcess
    {
    ServiceController::ServiceController(cstring name, cstring machine)
      :_name(name != nullptr ? name : L"")
      ,_serviceName()
      ,_machineName(machine != nullptr ? machine : L".")
      ,_displayName()
      {
      }
    ServiceController::~ServiceController()
      {
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
          return String(buffer);
          }
        }
      }
    void ServiceController::ValidateServiceName(String& serviceName)
      {
      if(serviceName.Length() == 0 || serviceName.Length() > 80)
        //throw new ArgumentException (string.Format (CultureInfo.CurrentCulture, "Service name {0} contains invalid characters, is empty" + " or is too long (max length = 80).", serviceName));
          throw ArgumentException(L"Service name is empty or contains invalid characters.");
      }
    }
  }