#pragma once

namespace Microsoft
  { 
  namespace VisualStudio
    { 
    namespace CppUnitTestFramework
      {
      template<> static std::wstring ToString<int64>(const int64& t)
        {
        Int64 i(t); 
        return std::wstring((cstring)i.ToString());
        }
      }
    }
  }