#pragma once
#include "System.String.h"
#include "AutoPtr.h"
#include "System.Globalization.CompareOptions.h"

namespace System
  {
  namespace Globalization
    {
    class CultureInfo;
    class CompareInfo
      {
      static const ulong ValidCompareOptions_NoStringSort = (ulong)CompareOptions::None | (ulong)CompareOptions::IgnoreCase | (ulong)CompareOptions::IgnoreNonSpace |
        (ulong)CompareOptions::IgnoreSymbols | (ulong)CompareOptions::IgnoreKanaType | (ulong)CompareOptions::IgnoreWidth | (ulong)CompareOptions::OrdinalIgnoreCase |
        (ulong)CompareOptions::Ordinal;
      static const ulong ValidCompareOptions = ValidCompareOptions_NoStringSort | (ulong)CompareOptions::StringSort;
      private:
        int    _culture;
        String _name;
      public:
        virtual ~CompareInfo();
        virtual int Compare(String& string1, String& string2);
        virtual int Compare(String& string1, String& string2, CompareOptions options);
        virtual int Compare(String& string1, int offset1, int length1, String& string2, int offset2, int length2, CompareOptions options);
        virtual bool IsSuffix(String& source, String& suffix);
        virtual bool IsSuffix(String& source, String& suffix, CompareOptions options);
      private:
        CompareInfo(CultureInfo&);
        int internal_compare_switch(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options);
        int internal_compare(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options);
        static int invariant_compare(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options);
        static int invariant_compare_char(wchar_t c1, wchar_t c2, CompareOptions options);

        friend CultureInfo;
      };

    typedef AutoPtr<CompareInfo> GCCompareInfo;
    }
  }

