#include "pch.h"
#include "System.Text.RegularExpressions.FactoryCache.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      FactoryCache::FactoryCache(int capacity)
        :_capicity(capacity)
        {
        }

      FactoryCache::~FactoryCache()
        {
        }
      }
    }
  }
