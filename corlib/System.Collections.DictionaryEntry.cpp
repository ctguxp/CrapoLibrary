#include "pch.h"
#include "System.Collections.DictionaryEntry.h"

namespace System
  {
  namespace Collections
    {
    DictionaryEntry::DictionaryEntry(Object* key, Object* value)
      :_key(key)
		  ,_value(value)
      {
      }
    DictionaryEntry::~DictionaryEntry()
      {
      }
    }
  }
