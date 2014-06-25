#pragma once
#include "System.Object.h"

namespace System
  {
  namespace Collections
    {
    class DictionaryEntry : public Object
      {
      private:
        Object* _key;
		    Object* _value;
      public:
        DictionaryEntry(Object* key, Object* value);
        ~DictionaryEntry();
      };
    }
  }
