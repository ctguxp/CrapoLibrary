#pragma once

namespace System
  {
  namespace ServiceProcess
    {
    class ServiceBase
      {
      public:
        ServiceBase();
        ~ServiceBase();
        bool AutoLog();
        void AutoLog(bool value);
        bool CanStop();
        void CanStop(bool value);
        bool CanPauseAndContinue();
        void CanPauseAndContinue(bool value);
        String ServiceName();
        void ServiceName(String value);
      private:
        bool   _hasStarted;
        bool   _auto_log;
        bool   _can_stop;
        bool   _can_pause_and_continue;
        String _service_name;
      };
    }
  }
