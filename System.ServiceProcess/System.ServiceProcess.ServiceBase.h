#pragma once
#include "System.ServiceProcess.ServiceControllerStatus.h"

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceBase
      {
      static bool share_process;
      public:
        ServiceBase();
        ServiceBase(ServiceBase const& sb);
        ~ServiceBase();
        ServiceBase& operator=(const ServiceBase& sb);
        bool AutoLog();
        void AutoLog(bool value);
        bool CanStop();
        void CanStop(bool value);
        bool CanPauseAndContinue();
        void CanPauseAndContinue(bool value);
        String& ServiceName();
        void ServiceName(String value);
        static void Run(ServiceBase& service);
        static void Run(Array<ServiceBase*>& services);
        void Stop();
        void ServiceMain(LPSERVICE_MAIN_FUNCTION serviceMain);
        LPSERVICE_MAIN_FUNCTION ServiceMain();
        void ServiceMainCallback(DWORD argCount, LPWSTR* argPointer);
      protected:
        virtual void OnStart(StringArray& args);
        virtual void OnStop();
        virtual void OnPause();
        virtual void OnContinue();
        virtual void OnShutdown();
      private:
        static void Win32RunService(Array<ServiceBase*>& services);
        static int WINAPI Win32HandlerFn(DWORD control, DWORD eventType, LPVOID eventData, LPVOID context);
        void SetStatus(ServiceControllerStatus status);
        static void Win32NotifyStatus(ServiceBase& service, ServiceControllerStatus status);
      private:
        bool                          _hasStarted;
        bool                          _auto_log;
        bool                          _can_stop;
        bool                          _can_pause_and_continue;
        bool                          _can_handle_power_event;
        bool                          _can_handle_session_change_event;
        bool                          _can_shutdown;
        SERVICE_STATUS_HANDLE         _service_handle;
        LPSERVICE_MAIN_FUNCTION       _service_main;
        Threading::GCManualResetEvent _stop_event;
        String                        _service_name;
      };
    }
  }
