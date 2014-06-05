#pragma once

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceManager
      {
      public:
        ServiceManager(String /*machineName*/, DWORD desiredAccess = SC_MANAGER_CONNECT);
        ~ServiceManager();
        operator SC_HANDLE() { return _handle; }
      private:
        SC_HANDLE _handle;
      };
    }
  }