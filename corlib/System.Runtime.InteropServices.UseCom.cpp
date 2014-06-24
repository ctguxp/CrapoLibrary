#include "pch.h"
#include "System.Runtime.InteropServices.UseCom.h"

namespace System
  {
  namespace Runtime
    {
    namespace InteropServices
      {
      UseCom::UseCom()
        {
        ::CoInitialize(NULL);
        }
      UseCom::~UseCom()
        {
        ::CoUninitialize();
        }

      UseComEx::UseComEx()
        {
        ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
        }

      UseComEx::~UseComEx()
        {
        ::CoUninitialize();
        }
      }
    }
  }
