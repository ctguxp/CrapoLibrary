#pragma once

namespace System
  {
  namespace Collections
    {
    class IDictionary
     {
      virtual bool IsFixedSize() = 0;
      virtual bool IsReadOnly() = 0;  
      };
    }
  }
