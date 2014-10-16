#include "pch.h"
#include "System.Collections.DictionaryEntry.h"

namespace System
  {
  namespace Collections
    {
    DictionaryEntry::DictionaryEntry(GCObject& key, GCObject& value)
      :_key(key)
		  ,_value(value)
      {
      }
    DictionaryEntry::~DictionaryEntry()
      {
      }
    }
  }
