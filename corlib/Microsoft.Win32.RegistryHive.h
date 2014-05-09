#pragma once

namespace Microsoft
  {
  namespace Win32
    {
    enum class RegistryHive : ULONG_PTR
      {
      NullHive        = 0,
      ClassesRoot     = (ULONG_PTR)((LONG)0x80000000),
		  CurrentConfig   = (ULONG_PTR)((LONG)0x80000005),
		  CurrentUser     = (ULONG_PTR)((LONG)0x80000001),
		  DynData         = (ULONG_PTR)((LONG)0x80000006),
		  LocalMachine    = (ULONG_PTR)((LONG)0x80000002),
		  PerformanceData = (ULONG_PTR)((LONG)0x80000004),
		  Users           = (ULONG_PTR)((LONG)0x80000003)
      };
    }
  }
