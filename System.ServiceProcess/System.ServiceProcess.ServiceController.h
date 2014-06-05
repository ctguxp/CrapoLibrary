#pragma once
#include "System.ServiceProcess.ServiceManager.h"

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceInstaller;
    class ServiceController
      {
      public:
        ServiceController(cstring name = nullptr, cstring machine = nullptr);
        ~ServiceController();
        String DisplayName();
        void DisplayName(String value);
        String ServiceName();
        void ServiceName(String value);
      protected:
        String Name();
      private:
        static String GetServiceName(ServiceManager& scHandle, String& displayName);
        static String GetServiceDisplayName(ServiceManager& scHandle, String& serviceName, String& machineName);
        static void ValidateServiceName(String& serviceName);
      private:
        String _name;
        String _serviceName;
        String _machineName;
        String _displayName;
      private:
        friend ServiceInstaller;
      };
    }
  }
