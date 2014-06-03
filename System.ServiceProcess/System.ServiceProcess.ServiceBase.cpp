#include "pch.h"
#include "System.ServiceProcess.ServiceBase.h"

namespace System
  {
  namespace ServiceProcess
    {
    ServiceBase::ServiceBase()
      :_hasStarted(false)
      ,_auto_log()
      ,_can_stop()
      ,_can_pause_and_continue()
      ,_service_name()
      {
      }
    ServiceBase::~ServiceBase()
      {
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
    String ServiceBase::ServiceName()
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
    }
  }