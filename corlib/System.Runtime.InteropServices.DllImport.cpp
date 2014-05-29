#include "pch.h"
#include "System.Runtime.InteropServices.DllImport.h"
#include "System.Exception.h"
#include "System.Text.ASCIIEncoding.h"

namespace System
  {
  namespace Runtime
    {
    namespace InteropServices
      {
      DllImport::DllImport(String dllName)
        :_instance(::LoadLibrary(dllName))
        ,_dllName(dllName)
        {
        if(_instance == NULL)
          throw SystemException(L"Cannot load dynamic library");
        }
      DllImport::~DllImport()
        {
        if(_instance != NULL)
          {
          ::FreeLibrary(_instance);
          _instance = NULL;
          }
        }
      void* DllImport::GetFunction(String& functionName)
        {
        using namespace Text;
        ASCIIEncoding ascii;
        int len = ascii.GetByteCount(functionName);
        ByteArray tempName = ascii.GetBytes(functionName);
        tempName.Length(len + 1);
        tempName[len] = '\0';
        void* retptr = ::GetProcAddress(_instance, (LPCSTR)tempName.ToConstPtr());
        if(retptr == NULL)
          throw SystemException(L"Cannot find function in the dynamic link library");
        return retptr;
        }
      }
    }
  }
