#include "pch.h"
#include "System.Runtime.InteropServices.UseCom.h"

namespace System
  {
  namespace Runtime
    {
    namespace InteropServices
      {
      UseOle::UseOle()
        {
        ::OleInitialize(NULL);
        }
      
      UseOle::~UseOle()
        {
        ::OleUninitialize();
        }

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
        ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        }

      UseComEx::~UseComEx()
        {
        ::CoUninitialize();
        }
      }
    }
  }
