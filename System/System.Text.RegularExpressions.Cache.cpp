#include "pch.h"
#include "System.Text.RegularExpressions.Cache.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      FactoryCache::FactoryCache(int capacity)
        :_capacity(capacity)
        ,_factories(capacity)
        {
        }

      FactoryCache::~FactoryCache()
        {
        }
      }
    }
  }
