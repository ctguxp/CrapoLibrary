#pragma once

namespace System
  {
  namespace ServiceProcess
    {
    enum class ServiceControllerStatus
      {
      Stopped = 1,
      StartPending = 2,
      StopPending = 3,
      Running = 4,
      ContinuePending = 5,
      PausePending = 6,
      Paused = 7
      };
    }
  }
