#pragma once
#include "System.ServiceProcess.ServiceManager.h"
#include "System.ServiceProcess.ServiceControllerStatus.h"

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceInstaller;
    class ServiceController
      {
      private:
        class ServiceHandle
        {
        public:
          ServiceHandle(String& name);
          ~ServiceHandle();
          operator SC_HANDLE() { return _handle; }
          void Open(ServiceManager& manager, DWORD dwDesiredAccess);
        private:
          String    _name;
          SC_HANDLE _handle;
        };
      public:
        ServiceController(cstring name = nullptr, cstring machine = nullptr);
        ~ServiceController();
        String DisplayName();
        void DisplayName(String value);
        String ServiceName();
        void ServiceName(String value);
        ServiceControllerStatus Status(); 
      protected:
        String Name();
      private:
        static SERVICE_STATUS_PROCESS GetServiceStatus(String& serviceName, String& machineName);
        static String GetServiceName(ServiceManager& scHandle, String& displayName);
        static String GetServiceDisplayName(ServiceManager& scHandle, String& serviceName, String& machineName);
        static void ValidateServiceName(String& serviceName);
      private:
        SERVICE_STATUS_PROCESS _status;
        String _name;
        String _serviceName;
        String _machineName;
        String _displayName;
      private:
        friend ServiceInstaller;
      };
    }
  }
