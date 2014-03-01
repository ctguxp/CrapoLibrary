// Perch Lake Computer System

#pragma once
#include "System.IFormatProvider.h"
#include "System.Globalization.NumberFormatInfo.h"
#include "System.String.h"
#include "CultureInfoEntry.h"
#include "System.Globalization.CompareInfo.h"
#include "System.Globalization.TextInfo.h"

#pragma warning(disable:4251)
namespace System
  {
  namespace Threading
    {
    class Thread;
    }
  namespace Globalization
    {
    class CRAPOCOREDLL_API CultureInfo : public IFormatProvider
      {
      enum : int32
        {
        NumOptionalCalendars = 5,
        GregorianTypeMask = 167772215,
        CalendarTypeBits = 24,
        InvariantCultureId = 0x7F
        };
      private:
        bool               _isReadOnly;
        uint32             _cultureID;
        uint32             _parent_lcid;
        uint32             _specific_lcid;
        int                _datetime_index;
        int                _number_index;
        int                _default_calendar_type;
        bool               _useUserOverride;
        GCNumberFormatInfo _numInfo;
        GCTextInfo         _textInfo;
        String             _name;
        String             _englishname;
        String             _nativename;
        String             _iso3lang;
        String             _iso2lang;
        String             _icu_name;
        String             _win3lang;
        GCCompareInfo      _compareInfo;
        void*              _textinfo_data;
        bool               _constructed;
      public:
        CultureInfo(uint32 = InvariantCultureId, bool useUserOverride = true);
        virtual ~CultureInfo();
        virtual Object GetFormat() override;
        virtual NumberFormatInfo* NumberFormat();
        static CultureInfo& CurrentCulture();
        virtual String Name();
        virtual int LCID();
        virtual CompareInfo& CompareInfo();
        virtual TextInfo& TextInfo();
      private:
        CultureInfo(uint32 culture, bool useUserOverride, bool readOnly);
        void Init();
        void ConstructInvariant();
        bool ConstructInternalLocaleFromLCID();
        bool construct_culture(const CultureInfoEntry* ci);
        Globalization::TextInfo* CreateTextInfo();
        void construct_number_format();



        static CultureInfo* ConstructCurrentCulture();
        String CurrentLocaleName();
        void Init3();

        friend Threading::Thread;
      };

    typedef AutoPtr<CultureInfo> GCCultureInfo;
    }
  }
#pragma warning(default:4251)
