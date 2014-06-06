#include "pch.h"
#include "System.ServiceProcess.ServiceBase.h"

namespace System
  {
  namespace ServiceProcess
    {
    bool ServiceBase::share_process = false;

    ServiceBase::ServiceBase()
      :_hasStarted(false)
      ,_auto_log()
      ,_can_stop()
      ,_can_pause_and_continue()
      ,_can_handle_power_event()
      ,_can_handle_session_change_event()
      ,_can_shutdown()
      ,_exitCode(0)
      ,_checkPoint(1)
      ,_service_handle(NULL)
      ,_stop_event()
      ,_service_name()
      {
      }
    ServiceBase::ServiceBase(ServiceBase const& sb)
      :_hasStarted(sb._hasStarted)
      ,_auto_log(sb._auto_log)
      ,_can_stop(sb._can_stop)
      ,_can_pause_and_continue(sb._can_pause_and_continue)
      ,_can_handle_power_event(sb._can_handle_power_event)
      ,_can_handle_session_change_event(sb._can_handle_session_change_event)
      ,_can_shutdown(sb._can_shutdown)
      ,_exitCode(sb._exitCode)
      ,_checkPoint(sb._checkPoint)
      ,_service_handle(sb._service_handle)
      ,_stop_event()
      ,_service_name(sb._service_name)
      {
      _stop_event.Set((Threading::ManualResetEvent*)sb._stop_event.Get());
      }
    ServiceBase::~ServiceBase()
      {
      }
    ServiceBase& ServiceBase::operator=(const ServiceBase& sb)
      {
      if(this == &sb)
        return *this;

      _hasStarted = sb._hasStarted;
      _auto_log = sb._auto_log;
      _can_stop = sb._can_stop;
      _can_pause_and_continue = sb._can_pause_and_continue;
      _can_handle_power_event = sb._can_handle_power_event;
      _can_handle_session_change_event = sb._can_handle_session_change_event;
      _can_shutdown = sb._can_shutdown;
      _exitCode = sb._exitCode;
      _checkPoint = sb._checkPoint;
      _service_handle = sb._service_handle;
      _stop_event.Set((Threading::ManualResetEvent*)sb._stop_event.Get());
      _service_name = sb._service_name;

      return *this;
      }
    bool ServiceBase::AutoLog()
      {
      return _auto_log;
      }
    void ServiceBase::AutoLog(bool value)
      {
      _auto_log = value;
      }
    bool ServiceBase::CanStop()
      {
      return _can_stop;
      }
    void ServiceBase::CanStop(bool value)
      {
      if(_hasStarted)
        //throw InvalidOperationException(L"Cannot modify this property after the service has started.");
          throw SystemException(L"Cannot modify this property after the service has started.");
      _can_stop = value;
      }
    bool ServiceBase::CanPauseAndContinue()
      {
      return _can_pause_and_continue;
      }
    void ServiceBase::CanPauseAndContinue(bool value)
      {
      if(_hasStarted)
        //throw InvalidOperationException(L"Cannot modify this property after the service has started.");
          throw SystemException(L"Cannot modify this property after the service has started.");
      _can_pause_and_continue = value;
      }
    int ServiceBase::ExitCode()
      {
      return _exitCode;
      }
    void ServiceBase::ExitCode(int value)
      {
      _exitCode = value;
      }
    uint32 ServiceBase::CheckPoint()
      {
      return _checkPoint;
      }
    void ServiceBase::CheckPoint(uint32 value)
      {
      _checkPoint = value;
      }
    String& ServiceBase::ServiceName()
      {
      return _service_name;
      }
    void ServiceBase::ServiceName(String value)
      {
      if(_hasStarted)
        //throw InvalidOperationException(L"Cannot modify this property after the service has started.");
          throw SystemException(L"Cannot modify this property after the service has started.");
      _service_name = value;
      }
    void ServiceBase::ServiceMain(LPSERVICE_MAIN_FUNCTION serviceMain)
      {
      _service_main = serviceMain;
      }
    LPSERVICE_MAIN_FUNCTION ServiceBase::ServiceMain()
      {
      return _service_main;
      }
    void ServiceBase::Stop()
      {
      if(_stop_event.Get() != nullptr)
        {
        //::OutputDebugString(L"CRAPO Stop SetEvent\r\n");
        _stop_event->Set();
        }
      else
        OnStop();
      }
    void ServiceBase::OnStart(StringArray& args)
      {
      }
    void ServiceBase::OnStop()
      {
      ::OutputDebugString(L"CRAPO OnStop\r\n");
      }
    void ServiceBase::OnPause()
      {
      }
    void ServiceBase::OnContinue()
      {
      }
    void ServiceBase::OnShutdown()
      {
      }
    void ServiceBase::Run(ServiceBase& service)
      {
      Array<ServiceBase*> sbArr(1);
      sbArr[0] = &service;
      Run(sbArr);
      }
    void ServiceBase::Run(Array<ServiceBase*>& services)
      {
      //int p = (int) Environment.OSVersion.Platform;

      //if (RunService != null)
      //RunService (services);
      //else if (!(p == 4 || p == 128 || p == 6))
      Win32RunService(services);
      //else
      //Console.Error.WriteLine("Use mono-service to start service processes");
      }
    void ServiceBase::ServiceMainCallback(DWORD argCount, LPWSTR* argPointer)
      {
      // handler needs to last until the service stops
      _service_handle = ::RegisterServiceCtrlHandlerEx(_service_name, (LPHANDLER_FUNCTION_EX)Win32HandlerFn, this);
      //::OutputDebugString(L"CRAPO RegisterServiceCtrlHandlerEx\r\n");

      if(_service_handle != NULL)
        {
        SetStatus(ServiceControllerStatus::StartPending);

        _stop_event.Set(new Threading::ManualResetEvent(false));

        StringArray args(argCount);
        for(uint32 i = 0; i < argCount; i++)
          {
          //IntPtr arg = Marshal.ReadIntPtr (argPointer, IntPtr.Size * i);
          //args[i] = Marshal.PtrToStringUni (arg);
          }

        OnStart(args);
        //::OutputDebugString(L"CRAPO OnStart\r\n");

        SetStatus(ServiceControllerStatus::Running);
        //::OutputDebugString(L"CRAPO Running\r\n");

        //::OutputDebugString(L"CRAPO Wait Start\r\n");
        _stop_event->WaitOne();
        //::OutputDebugString(L"CRAPO Wait End\r\n");

        SetStatus(ServiceControllerStatus::StopPending);

        OnStop();

        SetStatus(ServiceControllerStatus::Stopped);
        }
      }
    void ServiceBase::Win32RunService(Array<ServiceBase*>& services)
      {
      Array<SERVICE_TABLE_ENTRY> table(services.Length() + 1);

      sizet i = 0;
      for(; i < services.Length(); ++i)
        {
        table[i].lpServiceName = (LPWSTR)(LPCWSTR)services[i]->ServiceName();
        table[i].lpServiceProc = services[i]->ServiceMain();
        }

      // NULL termination
      table[i].lpServiceName = NULL;
      table[i].lpServiceProc = NULL;

      share_process = (services.Length() > 1);

      if(!StartServiceCtrlDispatcher(table.ToPtr()))
        //throw new Win32Exception ();
          throw WinException(L"StartServiceCtrlDispatcher Failed");
      }
    int WINAPI ServiceBase::Win32HandlerFn(DWORD control, DWORD eventType, LPVOID eventData, LPVOID context)
      {
      ServiceBase* obj = reinterpret_cast<ServiceBase*>(context);
      if(obj == nullptr)
        {
        //::OutputDebugString(L"CRAPO Bad Pointer\r\n");
        }
      switch(control)
        {
        case SERVICE_CONTROL_STOP:
          if((*obj)._can_stop)
            {
            //::OutputDebugString(L"CRAPO Handler Stop\r\n");
            (*obj).Stop();
            return NO_ERROR;
            }
          break;
        case SERVICE_CONTROL_PAUSE:
          if((*obj)._can_pause_and_continue)
            {
            (*obj).SetStatus(ServiceControllerStatus::PausePending);
            (*obj).OnPause();
            (*obj).SetStatus(ServiceControllerStatus::Paused);
            return NO_ERROR;
            }
          break;
        case SERVICE_CONTROL_CONTINUE:
          if((*obj)._can_pause_and_continue)
            {
            (*obj).SetStatus(ServiceControllerStatus::ContinuePending);
            (*obj).OnContinue();
            (*obj).SetStatus(ServiceControllerStatus::Running);
            return NO_ERROR;
            }
          break;
        case SERVICE_CONTROL_INTERROGATE:
          return NO_ERROR;
        case SERVICE_CONTROL_SHUTDOWN:
          if((*obj)._can_shutdown)
            {
            (*obj).OnShutdown();
            return NO_ERROR;
            }
          break;
        default:
          break;
        }
      return ERROR_CALL_NOT_IMPLEMENTED;
      }
    void ServiceBase::SetStatus(ServiceControllerStatus status)
      {
      if(!_hasStarted && status != ServiceControllerStatus::Stopped)
        _hasStarted = true;
      Win32NotifyStatus(*this, status);
      }
    void ServiceBase::Win32NotifyStatus(ServiceBase& service, ServiceControllerStatus status)
      {
      SERVICE_STATUS service_status;
      ZeroMemory(&service_status, sizeof(SERVICE_STATUS));

      service_status.dwServiceType = share_process ? SERVICE_WIN32_SHARE_PROCESS : SERVICE_WIN32_OWN_PROCESS;

      service_status.dwCurrentState = (int)status;
      service_status.dwWin32ExitCode = service.ExitCode();
      service_status.dwWaitHint = 5000;

      if(status != ServiceControllerStatus::StartPending)
        {
        if(service._can_stop)
          service_status.dwControlsAccepted |= SERVICE_ACCEPT_STOP;

        if(service._can_pause_and_continue)
          service_status.dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;

        if(service._can_handle_power_event)
          service_status.dwControlsAccepted |= SERVICE_ACCEPT_POWEREVENT;

        if(service._can_handle_session_change_event)
          service_status.dwControlsAccepted |= SERVICE_ACCEPT_SESSIONCHANGE;

        if(service._can_shutdown)
          service_status.dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
        }

      if(status == ServiceControllerStatus::Running || status == ServiceControllerStatus::Stopped)
        service_status.dwCheckPoint = 0;
      else
        {
        service_status.dwCheckPoint = service.CheckPoint();
        service.CheckPoint(service.CheckPoint() + 1);
        }

      ::SetServiceStatus(service._service_handle, &service_status);
      }
    }
  }