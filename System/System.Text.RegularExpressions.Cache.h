#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class FactoryCache
        {
        public:
          FactoryCache(int /*capicity*/);
          ~FactoryCache();
        private:
          int _capacity;
          Collections::Hashtable _factories;
        };
      }
    }
  }
