#pragma once
#include "System.String.h"

namespace System
  {
  namespace Runtime
    {
    namespace InteropServices
      {
      class CRAPOCOREDLL_API DllImport
        {
        public:
          DllImport(String dllName);
          ~DllImport();
          String FileName() { return _dllName; }
          
          template<class T>
          void GetFunction(String functionName, T& funcPointer)
          {
          funcPointer = static_cast<T>(GetFunction(functionName));
          }
        
        private:
          void* GetFunction(String& functionName);
        private:
          HINSTANCE _instance;
          String    _dllName;
        };
      }
    }
  }

