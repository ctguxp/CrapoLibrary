#pragma once
#include "System.ServiceProcess.ServiceStartMode.h"

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceInstaller
      {
      public:
        ServiceInstaller();
        ~ServiceInstaller();
        String DisplayName();
        void DisplayName(String value);
        String ServiceName();
        void ServiceName(String value);
        void Install(String path);
        void Uninstall();
      private:
        ServiceStartMode _startType;
        String           _serviceName;
        String           _displayName;
      };
    }
  }
