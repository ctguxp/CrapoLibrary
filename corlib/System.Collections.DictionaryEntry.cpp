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
    GCObject DictionaryEntry::Key()
      {
      return _key;
      }
    void DictionaryEntry::Key(GCObject value)
      {
      _key = value;
      }
		GCObject DictionaryEntry::Value()
      {
      return _value;
      }
    void DictionaryEntry::Value(GCObject value)
      {
      _value = value;
      }
    }
  }
