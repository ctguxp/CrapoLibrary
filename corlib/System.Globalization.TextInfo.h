#pragma once
#include "AutoPtr.h"

namespace System
  {
  namespace Globalization
    {
    class CultureInfo;
    class TextInfo
      {
      private:
        struct Data
          {
          int ansi;
          int ebcdic;
          int mac;
          int oem;
          bool right_to_left;
          byte list_sep;
          };
      private:
        bool         _isReadOnly;
        int          _win32LangID;
        CultureInfo* _ci;
        Data         _data;
      public:
        virtual ~TextInfo();
         virtual wchar_t ToLower(wchar_t);
      private:
        TextInfo(CultureInfo* ci, int lcid, void* data, bool read_only);

      friend CultureInfo;
      };

    typedef AutoPtr<TextInfo> GCTextInfo;
    }
  }
